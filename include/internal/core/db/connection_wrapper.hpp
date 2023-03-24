#pragma once

#include "connection.hpp"

namespace fstore::core::db {
    class connection_wrapper {
        std::shared_ptr<connection> inner;
    public:
        connection_wrapper(std::shared_ptr<connection>&& inner);

        auto add_error(
            const UUID::uuid& object_id,
            std::string_view message
        ) -> ext::task<>;

        auto add_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::string_view hash,
            std::int64_t size,
            std::string_view type,
            std::string_view subtype
        ) -> ext::task<object>;

        auto clear_error(const UUID::uuid& object_id) -> ext::task<>;

        auto create_bucket(
            std::string_view name
        ) -> ext::task<bucket>;

        auto fetch_bucket(
            std::string_view name
        ) -> ext::task<bucket>;

        auto fetch_buckets() ->
            ext::task<std::vector<bucket>>;

        auto fetch_buckets(
            std::span<const std::string> names
        ) -> ext::task<std::vector<bucket>>;

        auto fetch_store_totals() -> ext::task<store_totals>;

        auto get_errors() -> ext::task<std::vector<object_error>>;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        auto get_objects(
            int batch_size
        ) -> ext::task<pg::portal<object>>;

        auto remove_bucket(const UUID::uuid& id) -> ext::task<>;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<remove_result>;

        auto remove_orphan_objects() ->
            ext::task<std::vector<object>>;

        auto rename_bucket(
            const UUID::uuid& id,
            std::string_view name
        ) -> ext::task<>;
    };
}
