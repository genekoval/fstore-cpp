#include <internal/core/db/database.hpp>
#include <internal/core/fs/filesystem.hpp>
#include <internal/core/object_store.hpp>

#include <fstore/except.hpp>

#include <ext/string.h>

namespace {
    auto generate_random_uuid() -> UUID::uuid {
        return UUID::generate();
    }

    auto process_bucket_name(std::string_view name) -> std::string_view {
        const auto trimmed = ext::trim(name);

        if (trimmed.empty()) {
            throw fstore::invalid_data("bucket name cannot be empty");
        }

        return trimmed;
    }
}

namespace fstore::core {
    object_store::object_store(
        db::database& database,
        fs::filesystem& filesystem
    ) :
        database(&database),
        filesystem(&filesystem),
        generate_uuid(generate_random_uuid)
    {}

    object_store::object_store(
        db::database& database,
        fs::filesystem& filesystem,
        uuid_generator generate_uuid
    ) :
        database(&database),
        filesystem(&filesystem),
        generate_uuid(generate_uuid)
    {}

    auto object_store::check(
        int batch_size,
        int jobs,
        check_progress& progress
    ) -> ext::task<std::size_t> {
        auto db = co_await database->connect();

        progress.total = (co_await db.fetch_store_totals()).objects;
        std::size_t errors = 0;

        auto workers = netcore::thread_pool("worker", jobs);
        auto counter = ext::counter();
        auto objects = co_await db.get_objects(batch_size);

        while (objects) {
            for (const auto& obj : co_await objects.next()) {
                check_object_task(obj, workers, errors, progress, counter);
            }

            co_await counter;
        }

        co_return errors;
    }

    auto object_store::check_object(
        const db::object& obj
    ) noexcept -> std::string {
        try {
            const auto path = filesystem->object_path(obj.id);
            if (!std::filesystem::exists(path)) return "file missing";

            const auto hash = filesystem->hash(path);
            if (hash != obj.hash) return "hash mismatch";

            return std::string();
        }
        catch (const std::exception& ex) {
            return ex.what();
        }
        catch (...) {
            return "unknown error";
        }
    }

    auto object_store::check_object_task(
        const db::object obj,
        netcore::thread_pool& workers,
        std::size_t& errors,
        check_progress& progress,
        ext::counter& counter
    ) -> ext::detached_task {
        const auto counter_item = counter.increment();

        const auto message = co_await workers.wait([this, &obj]() {
            return check_object(obj);
        });

        auto db = co_await database->connect();

        if (message.empty()) co_await db.clear_error(obj.id);
        else {
            ++errors;
            co_await db.add_error(obj.id, message);
        }

        ++progress.completed;
    }

    auto object_store::commit_part(
        const UUID::uuid& bucket_id,
        const UUID::uuid& part_id
    ) -> ext::task<object> {
        auto db = co_await database->connect();

        auto part = filesystem->part_path(part_id);
        const auto mime = filesystem->mime_type(part);

        auto obj = co_await db.add_object(
            bucket_id,
            part_id,
            filesystem->hash(part),
            filesystem->size(part),
            mime.type,
            mime.subtype
        );

        // This object was uploaded previously.
        if (obj.id != part_id) filesystem->remove_part(part_id);
        else filesystem->make_object(part_id);

        co_return obj;
    }

    auto object_store::create_bucket(
        std::string_view name
    ) -> ext::task<bucket> {
        auto db = co_await database->connect();

        const auto processed_name = process_bucket_name(name);
        co_return co_await db.create_bucket(processed_name);
    }

    auto object_store::fetch_bucket(
        std::string_view name
    ) -> ext::task<bucket> {
        auto db = co_await database->connect();
        co_return co_await db.fetch_bucket(name);
    }

    auto object_store::fetch_buckets() -> ext::task<std::vector<bucket>> {
        auto db = co_await database->connect();
        auto buckets = co_await db.fetch_buckets();

        auto result = std::vector<bucket>();
        for (auto&& bucket : buckets) result.emplace_back(std::move(bucket));
        co_return result;
    }

    auto object_store::fetch_buckets(
        std::span<const std::string> names
    ) -> ext::task<std::vector<bucket>> {
        auto db = co_await database->connect();
        auto buckets = co_await db.fetch_buckets(names);

        auto result = std::vector<bucket>();
        for (auto&& bucket : buckets) result.emplace_back(std::move(bucket));
        co_return result;
    }

    auto object_store::fetch_store_totals() -> ext::task<store_totals> {
        auto db = co_await database->connect();
        co_return co_await db.fetch_store_totals();
    }

    auto object_store::get_errors() -> ext::task<std::vector<object_error>> {
        auto db = co_await database->connect();
        co_return co_await db.get_errors();
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<file> {
        auto meta = co_await get_object_metadata(bucket_id, object_id);
        co_return file { filesystem->open(object_id), meta.size };
    }

    auto object_store::get_object_metadata(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        auto db = co_await database->connect();
        co_return co_await db.get_object(bucket_id, object_id);
    }

    auto object_store::get_part(
        std::optional<UUID::uuid> part_id
    ) -> fs::part {
        const auto id = part_id.value_or(generate_uuid());
        return fs::part(id, filesystem->get_part(id));
    }

    auto object_store::prune() -> ext::task<std::vector<object>> {
        auto db = co_await database->connect();

        auto orphans = co_await db.remove_orphan_objects();
        for (const auto& obj : orphans) filesystem->remove(obj.id);

        TIMBER_INFO("Pruned {} objects", orphans.size());

        auto result = std::vector<object>();
        for (auto&& obj : orphans) result.emplace_back(std::move(obj));
        co_return result;
    }

    auto object_store::remove_bucket(
        const UUID::uuid& bucket_id
    ) -> ext::task<> {
        auto db = co_await database->connect();
        co_await db.remove_bucket(bucket_id);
    }

    auto object_store::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        auto db = co_await database->connect();
        co_return co_await db.remove_object(bucket_id, object_id);
    }

    auto object_store::remove_objects(
        const UUID::uuid& bucket_id,
        const std::vector<UUID::uuid>& objects
    ) -> ext::task<remove_result> {
        auto db = co_await database->connect();
        co_return co_await db.remove_objects(bucket_id, objects);
    }

    auto object_store::rename_bucket(
        const UUID::uuid& bucket_id,
        std::string_view bucket_name
    ) -> ext::task<> {
        auto db = co_await database->connect();

        const auto name = process_bucket_name(bucket_name);
        co_await db.rename_bucket(bucket_id, name);
    }
}
