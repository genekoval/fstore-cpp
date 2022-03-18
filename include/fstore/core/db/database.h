#pragma once

#include "model.h"

#include <fstore/test.h>

#include <pqxx/pqxx>
#include <string>
#include <string_view>

namespace fstore::core::db {
    class database {
        std::optional<pqxx::connection> connection;

        auto ntx() -> pqxx::nontransaction;
    public:
        database() = default;

        database(std::string_view connection_string);

        database(const database&) = delete;

        database(database&& other)  = default;

        VIRTUAL_DESTRUCTOR(database);

        VIRTUAL auto add_object(
            std::string_view bucket_id,
            std::string_view object_id,
            std::string_view hash,
            uintmax_t size,
            std::string_view type,
            std::string_view subtype
        ) -> object;

        VIRTUAL auto create_bucket(
            std::string_view bucket_id,
            std::string_view name
        ) -> bucket;

        VIRTUAL auto fetch_bucket(std::string_view name) -> bucket;

        VIRTUAL auto fetch_buckets() -> std::vector<bucket>;

        VIRTUAL auto fetch_buckets(
            const std::vector<std::string>& names
        ) -> std::vector<bucket>;

        VIRTUAL auto fetch_store_totals() -> store_totals;

        VIRTUAL auto get_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> std::optional<object>;

        VIRTUAL auto remove_bucket(std::string_view id) -> void;

        VIRTUAL auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> object;

        VIRTUAL auto remove_objects(
            std::string_view bucket_id,
            const std::vector<std::string>& objects
        ) -> remove_result;

        VIRTUAL auto remove_orphan_objects() -> std::vector<object>;

        VIRTUAL auto rename_bucket(std::string_view id, std::string_view name) -> void;
    };
}
