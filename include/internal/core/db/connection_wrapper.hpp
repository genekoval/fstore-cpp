#pragma once

#include "connection.hpp"

namespace fstore::core::db {
    class connection_wrapper : connection {
        std::shared_ptr<connection> inner;
    public:
        connection_wrapper(std::shared_ptr<connection>&& inner);

        auto add_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::string_view hash,
            std::int64_t size,
            std::string_view type,
            std::string_view subtype
        ) -> ext::task<object> override;

        auto create_bucket(
            std::string_view name
        ) -> ext::task<bucket> override;

        auto fetch_bucket(
            std::string_view name
        ) -> ext::task<bucket> override;

        auto fetch_buckets() ->
            ext::task<std::vector<bucket>> override;

        auto fetch_buckets(
            std::span<const std::string> names
        ) -> ext::task<std::vector<bucket>> override;

        auto fetch_store_totals() -> ext::task<store_totals> override;

        auto get_errors() -> ext::task<std::vector<object_error>> override;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object> override;

        auto get_objects(
            int batch_size
        ) -> ext::task<pg::portal<object>> override;

        auto remove_bucket(const UUID::uuid& id) -> ext::task<> override;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object> override;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<remove_result> override;

        auto remove_orphan_objects() ->
            ext::task<std::vector<object>> override;

        auto rename_bucket(
            const UUID::uuid& id,
            std::string_view name
        ) -> ext::task<> override;

        auto update_object_errors(
            std::span<const object_error> records
        ) -> ext::task<> override;
    };
}
