#include <fstore/service/object_store.h>

#include <filesystem>
#include <timber/timber>
#include <uuid++/uuid.h>

namespace fstore::service {
    object_store::object_store(
        std::string_view db_connection,
        std::string_view objects_dir
    ) :
        db(db_connection),
        fs(objects_dir)
    {
        INFO() << "Database connection: " << db_connection;
        INFO() << "Object storage: " << std::filesystem::canonical(objects_dir);
    }

    object_store::object_store(repo::db&& db, repo::fs&& fs) :
        db(std::move(db)),
        fs(std::move(fs))
    {}

    auto object_store::add_object(
        std::string_view bucket_id,
        std::string_view path
    ) -> model::object {
        auto uuid = UUID::uuid();
        uuid.generate();

        auto object = model::object {
            .id = uuid.string(),
            .hash = fs.hash(path),
            .size = fs.size(path),
            .mime_type = fs.mime_type(path)
        };

        db.add_object(bucket_id, object);
        fs.copy(path, object.id);

        return object;
    }

    auto object_store::create_bucket(std::string_view name) -> model::bucket {
        auto uuid = UUID::uuid();
        uuid.generate();

        auto bucket = model::bucket {
            .id = uuid.string(),
            .name = std::string(name)
        };

        db.create_bucket(bucket);
        INFO() << "Created bucket: " << bucket.name;

        return bucket;
    }

    auto object_store::fetch_bucket(std::string_view name) -> model::bucket {
        return db.fetch_bucket(name);
    }

    auto object_store::fetch_buckets() -> std::vector<model::bucket> {
        return db.fetch_buckets();
    }

    auto object_store::fetch_buckets(
        const std::vector<std::string_view>& names
    ) -> std::vector<model::bucket> {
        return db.fetch_buckets(names);
    }

    auto object_store::fetch_store_totals() -> model::object_store {
        return db.fetch_store_totals();
    }

    auto object_store::get_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> std::optional<std::pair<model::object, netcore::fd>> {
        auto meta = get_object_metadata(bucket_id, object_id);
        if (!meta) return {};

        return std::make_pair(meta.value(), fs.open(object_id));
    }

    auto object_store::get_object_metadata(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> std::optional<model::object> {
        return db.get_object(bucket_id, object_id);
    }

    auto object_store::prune() -> std::vector<model::object> {
        auto orphans = db.remove_orphan_objects();
        for (const auto& obj : orphans) fs.remove(obj.id);

        INFO() << "Pruned " << orphans.size() << " objects";

        return orphans;
    }

    auto object_store::remove_bucket(std::string_view bucket_id) -> void {
        db.remove_bucket(bucket_id);
    }

    auto object_store::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> model::object {
        return db.remove_object(bucket_id, object_id);
    }

    auto object_store::rename_bucket(
        std::string_view bucket_id,
        std::string_view bucket_name
    ) -> void {
        db.rename_bucket(bucket_id, bucket_name);
    }
}
