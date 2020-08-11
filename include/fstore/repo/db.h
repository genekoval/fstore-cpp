#pragma once

#include <fstore/models.h>

#include <pqxx/pqxx>
#include <string>
#include <string_view>

namespace fstore::repo {
    class db {
        pqxx::connection connection;
    public:
        db(std::string_view connection_string);

        auto add_object(
            std::string_view bucket_id,
            model::object& obj
        ) -> void;

        auto create_bucket(model::bucket& bkt) -> void;

        auto fetch_bucket(std::string_view bucket_name) -> model::bucket;

        auto fetch_buckets() -> std::vector<model::bucket>;

        auto fetch_buckets(
            const std::vector<std::string_view>& names
        ) -> std::vector<model::bucket>;

        auto fetch_store_totals() -> model::object_store;

        auto remove_bucket(std::string_view bucket_id) -> void;

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> model::object;

        auto remove_orphan_objects() -> std::vector<model::object>;

        auto rename_bucket(
            std::string_view bucket_id,
            std::string_view bucket_name
        ) -> void;
    };
}
