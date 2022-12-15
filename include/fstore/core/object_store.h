#pragma once

#include <fstore/core/model.h>
#include <fstore/core/part.h>

#include <optional>
#include <span>

namespace fstore::core {
    namespace db {
        class database;
    }

    namespace fs {
        class filesystem;
    }

    class object_store {
        using uuid_generator = auto (*)() -> UUID::uuid;

        db::database* database;
        fs::filesystem* filesystem;
        uuid_generator generate_uuid;

        auto check_object(const object& obj) -> bool;
    public:
        object_store(db::database& database, fs::filesystem& filesystem);

        object_store(
            db::database& database,
            fs::filesystem& filesystem,
            uuid_generator generate_uuid
        );

        auto add_object(
            const UUID::uuid& bucket_id,
            std::string_view path
        ) -> object;

        auto check(int jobs, check_progress& progress) -> std::size_t;

        auto commit_part(
            const UUID::uuid& bucket_id,
            const UUID::uuid& part_id
        ) -> object;

        auto create_bucket(std::string_view name) -> bucket;

        auto fetch_bucket(std::string_view name) -> bucket;

        auto fetch_buckets() -> std::vector<bucket>;

        auto fetch_buckets(
            const std::vector<std::string>& names
        ) -> std::vector<bucket>;

        auto fetch_store_totals() -> store_totals;

        auto get_errors() -> std::vector<object_error>;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> file;

        auto get_object_metadata(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> object;

        auto get_part(std::optional<UUID::uuid> part_id) -> part;

        auto prune() -> std::vector<object>;

        auto remove_bucket(const UUID::uuid& bucket_id) -> void;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> object;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ) -> remove_result;

        auto rename_bucket(
            const UUID::uuid& bucket_id,
            std::string_view bucket_name
        ) -> void;
    };
}
