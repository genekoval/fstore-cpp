#pragma once

#include <fstore/repo/db.h>
#include <fstore/repo/filesystem.h>

#include <optional>

namespace fstore::service {
    class object_store {
        repo::db db;
        repo::fs fs;
    public:
        object_store(
            std::string_view db_connection,
            std::string_view objects_dir
        );

        object_store(
            repo::db&& db,
            repo::fs&& fs
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

        auto get_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> std::optional<std::pair<model::object, netcore::fd>>;

        auto get_object_metadata(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> std::optional<model::object>;

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
