#include <fstore/core/db/database.h>
#include <fstore/core/filesystem.h>
#include <fstore/core/object_store.h>
#include <fstore/error.h>

#include <ext/string.h>
#include <threadpool/threadpool>
#include <uuid++/uuid.h>

namespace {
    auto generate_random_uuid() -> UUID::uuid {
        return UUID::generate();
    }

    auto process_bucket_name(std::string_view name) -> std::string {
        const auto trimmed = ext::trim(std::string(name));

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

    auto object_store::add_object(
        const UUID::uuid& bucket_id,
        std::string_view path
    ) -> object {
        const auto mime = filesystem->mime_type(path);

        const auto obj = database->add_object(
            bucket_id,
            generate_uuid(),
            filesystem->hash(path),
            filesystem->size(path),
            mime.type,
            mime.subtype
        );

        filesystem->copy(path, obj.id);
        return obj;
    }

    auto object_store::check_object(const object& obj) -> bool {
        try {
            const auto path = filesystem->object_path(obj.id);
            if (!std::filesystem::exists(path)) {
                database->add_error(obj.id, "file missing");
                return false;
            }

            const auto hash = filesystem->hash(path);
            if (hash != obj.hash) {
                database->add_error(obj.id, "hash mismatch");
                return false;
            }

            database->clear_error(obj.id);
            return true;
        }
        catch (const std::exception& ex) {
            database->add_error(obj.id, ex.what());
        }

        return false;
    }

    auto object_store::check(
        int jobs,
        check_progress& progress
    ) -> std::size_t {
        progress.total = database->fetch_store_totals().objects;
        std::atomic_ulong errors = 0;

        auto workers = threadpool::pool(jobs);

        auto objects = database->get_objects(100);

        while (objects) {
            for (auto&& obj : objects()) {
                workers.run([this, obj, &errors, &progress]() {
                    const auto success = check_object(obj);
                    if (!success) errors += 1;
                    progress.completed += 1;
                });
            }
        }

        return errors;
    }

    auto object_store::commit_part(
        const UUID::uuid& bucket_id,
        const UUID::uuid& part_id
    ) -> object {
        auto part = filesystem->part_path(part_id);

        const auto mime = filesystem->mime_type(part);

        const auto obj = database->add_object(
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

        return obj;
    }

    auto object_store::create_bucket(std::string_view name) -> bucket {
        const auto processed_name = process_bucket_name(name);

        return database->create_bucket(processed_name);
    }

    auto object_store::fetch_bucket(std::string_view name) -> bucket {
        return database->fetch_bucket(name);
    }

    auto object_store::fetch_buckets() -> std::vector<bucket> {
        return database->fetch_buckets();
    }

    auto object_store::fetch_buckets(
        const std::vector<std::string>& names
    ) -> std::vector<bucket> {
        return database->fetch_buckets(names);
    }

    auto object_store::fetch_store_totals() -> store_totals {
        return database->fetch_store_totals();
    }

    auto object_store::get_errors() -> std::vector<object_error> {
        return database->get_errors();
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> file {
        auto meta = get_object_metadata(bucket_id, object_id);

        return file { filesystem->open(object_id), meta.size };
    }

    auto object_store::get_object_metadata(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> object {
        return database->get_object(bucket_id, object_id);
    }

    auto object_store::get_part(
        std::optional<UUID::uuid> part_id
    ) -> part {
        const auto id = part_id.value_or(generate_uuid());
        return part(id, filesystem->get_part(id));
    }

    auto object_store::prune() -> std::vector<object> {
        auto orphans = database->remove_orphan_objects();
        for (const auto& obj : orphans) filesystem->remove(obj.id);

        TIMBER_INFO("Pruned {} objects", orphans.size());

        return orphans;
    }

    auto object_store::remove_bucket(const UUID::uuid& bucket_id) -> void {
        database->remove_bucket(bucket_id);
    }

    auto object_store::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> object {
        return database->remove_object(bucket_id, object_id);
    }

    auto object_store::remove_objects(
        const UUID::uuid& bucket_id,
        const std::vector<UUID::uuid>& objects
    ) -> remove_result {
        return database->remove_objects(bucket_id, objects);
    }

    auto object_store::rename_bucket(
        const UUID::uuid& bucket_id,
        std::string_view bucket_name
    ) -> void {
        const auto name = process_bucket_name(bucket_name);
        database->rename_bucket(bucket_id, name);
    }
}
