#pragma once

#include <fstore/repo/db.h>
#include <fstore/repo/filesystem.h>

namespace fstore::service {
    class object_store {
        repo::db db;
        repo::fs::fs_t fs;
    public:
        object_store(
            std::string_view db_connection,
            std::string_view objects_dir
        );

        object_store(
            repo::db&& db,
            repo::fs::fs_t&& fs
        );

        auto add_object(
            std::string_view bucket_id,
            std::string_view path
        ) -> model::object;

        auto create_bucket(std::string_view name) -> model::bucket;

        auto fetch_bucket(std::string_view name) -> model::bucket;

        auto fetch_buckets() -> std::vector<model::bucket>;

        auto fetch_buckets(
            const std::vector<std::string_view>& names
        ) -> std::vector<model::bucket>;

        auto fetch_store_totals() -> model::object_store;

        auto prune() -> std::vector<model::object>;

        auto remove_bucket(std::string_view bucket_id) -> void;

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> model::object;

        auto rename_bucket(
            std::string_view bucket_id,
            std::string_view bucket_name
        ) -> void;
    };
}
