#pragma once

#include "model.h"

#include <pqxx/pqxx>
#include <string>
#include <string_view>

namespace fstore::core::db {
    class database {
        pqxx::connection connection;

        auto ntx() -> pqxx::nontransaction;
    public:
        database(std::string_view connection_string);

        auto add_object(
            std::string_view bucket_id,
            std::string_view object_id,
            std::string_view hash,
            uintmax_t size,
            std::string mime_type
        ) -> object;

        auto add_object(std::string_view bucket_id, const object& obj) -> void;

        auto create_bucket(
            std::string_view bucket_id,
            std::string_view name
        ) -> bucket;

        auto fetch_bucket(std::string_view name) -> bucket;

        auto fetch_buckets() -> std::vector<bucket>;

        auto fetch_buckets(
            const std::vector<std::string>& names
        ) -> std::vector<bucket>;

        auto fetch_store_totals() -> store_totals;

        auto get_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> std::optional<object>;

        auto remove_bucket(std::string_view id) -> void;

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> object;

        auto remove_objects(
            std::string_view bucket_id,
            const std::vector<std::string>& objects
        ) -> remove_result;

        auto remove_orphan_objects() -> std::vector<object>;

        auto rename_bucket(std::string_view id, std::string_view name) -> void;
    };
}
