#pragma once

#include <fstore/core/model.h>
#include <fstore/core/part.h>
#include <fstore/repo/database.h>

#include <optional>
#include <span>

namespace fstore::core {
    class filesystem;

    class object_store {
        repo::database db;
        filesystem* fs;
    public:
        object_store(repo::database&& db, filesystem& fs);

        auto add_object(
            std::string_view bucket_id,
            std::string_view path
        ) -> object;

        auto commit_part(
            std::string_view bucket_id,
            std::string_view part_id
        ) -> object;

        auto create_bucket(std::string_view name) -> bucket;

        auto fetch_bucket(std::string_view name) -> bucket;

        auto fetch_buckets() -> std::vector<bucket>;

        auto fetch_buckets(
            const std::vector<std::string>& names
        ) -> std::vector<bucket>;

        auto fetch_store_totals() -> store_totals;

        auto get_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> std::optional<file>;

        auto get_object_metadata(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> std::optional<object>;

        auto get_part(std::optional<std::string_view> part_id) -> part;

        auto prune() -> std::vector<object>;

        auto remove_bucket(std::string_view bucket_id) -> void;

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> object;

        auto remove_objects(
            std::string_view bucket_id,
            const std::vector<std::string>& objects
        ) -> remove_result;

        auto rename_bucket(
            std::string_view bucket_id,
            std::string_view bucket_name
        ) -> void;
    };
}
