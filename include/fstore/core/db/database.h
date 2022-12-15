#pragma once

#include "model.h"

#include <fstore/test.h>

#include <entix/entix>
#include <pqxx/pqxx>
#include <string>
#include <string_view>

namespace fstore::core::db {
    class database {
        entix::connection_pool connections;
    public:
        database() = default;

        database(std::string_view connection_string, int connection_count);

        VIRTUAL_DESTRUCTOR(database);

        VIRTUAL auto add_error(
            const UUID::uuid& object_id,
            std::string_view message
        ) -> void;

        VIRTUAL auto add_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::string_view hash,
            uintmax_t size,
            std::string_view type,
            std::string_view subtype
        ) -> object;

        VIRTUAL auto clear_error(const UUID::uuid& object_id) -> void;

        VIRTUAL auto create_bucket(std::string_view name) -> bucket;

        VIRTUAL auto fetch_bucket(std::string_view name) -> bucket;

        VIRTUAL auto fetch_buckets() -> std::vector<bucket>;

        VIRTUAL auto fetch_buckets(
            const std::vector<std::string>& names
        ) -> std::vector<bucket>;

        VIRTUAL auto fetch_store_totals() -> store_totals;

        VIRTUAL auto get_errors() -> std::vector<object_error>;

        VIRTUAL auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> object;

        VIRTUAL auto get_objects(
            int batch_size
        ) -> ext::generator<std::span<object>>;

        VIRTUAL auto remove_bucket(std::string_view id) -> void;

        VIRTUAL auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> object;

        VIRTUAL auto remove_objects(
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ) -> remove_result;

        VIRTUAL auto remove_orphan_objects() -> std::vector<object>;

        VIRTUAL auto rename_bucket(
            const UUID::uuid& id,
            std::string_view name
        ) -> void;
    };
}
