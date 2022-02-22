#include <fstore/core/db/database.h>
#include <fstore/core/filesystem.h>
#include <fstore/core/object_store.h>

#include <timber/timber>
#include <uuid++/uuid.h>

namespace fstore::core {
    object_store::object_store(db::database& database, filesystem& fs) :
        database(&database),
        fs(&fs)
    {}

    auto object_store::add_object(
        std::string_view bucket_id,
        std::string_view path
    ) -> object {
        auto uuid = UUID::uuid();
        uuid.generate();

        const auto obj = database->add_object(
            bucket_id,
            uuid.string(),
            fs->hash(path),
            fs->size(path),
            fs->mime_type(path)
        );

        fs->copy(path, obj.id);
        return obj;
    }

    auto object_store::commit_part(
        std::string_view bucket_id,
        std::string_view part_id
    ) -> object {
        auto part = fs->part_path(part_id);

        const auto obj = database->add_object(
            bucket_id,
            part_id,
            fs->hash(part),
            fs->size(part),
            fs->mime_type(part)
        );

        // This object was uploaded previously.
        if (obj.id != part_id) fs->remove_part(part_id);
        else fs->make_object(part_id);

        return obj;
    }

    auto object_store::create_bucket(std::string_view name) -> bucket {
        auto uuid = UUID::uuid();
        uuid.generate();

        const auto bkt = database->create_bucket(uuid.string(), name);
        INFO() << "Created bucket: " << bkt.name;

        return bkt;
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

    auto object_store::get_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> std::optional<file> {
        auto meta = get_object_metadata(bucket_id, object_id);
        if (!meta) return {};

        return file { fs->open(object_id), meta.value().size };
    }

    auto object_store::get_object_metadata(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> std::optional<object> {
        return database->get_object(bucket_id, object_id);
    }

    auto object_store::get_part(
        std::optional<std::string_view> part_id
    ) -> part {
        std::string id;

        if (part_id) id = *part_id;
        else {
            auto uuid = UUID::uuid();
            uuid.generate();
            id = uuid.string();
        }

        return part(id, fs->get_part(id));
    }

    auto object_store::prune() -> std::vector<object> {
        auto orphans = database->remove_orphan_objects();
        for (const auto& obj : orphans) fs->remove(obj.id);

        INFO() << "Pruned " << orphans.size() << " objects";

        return orphans;
    }

    auto object_store::remove_bucket(std::string_view bucket_id) -> void {
        database->remove_bucket(bucket_id);
    }

    auto object_store::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> object {
        return database->remove_object(bucket_id, object_id);
    }

    auto object_store::remove_objects(
        std::string_view bucket_id,
        const std::vector<std::string>& objects
    ) -> remove_result {
        return database->remove_objects(bucket_id, objects);
    }

    auto object_store::rename_bucket(
        std::string_view bucket_id,
        std::string_view bucket_name
    ) -> void {
        database->rename_bucket(bucket_id, bucket_name);
    }
}
