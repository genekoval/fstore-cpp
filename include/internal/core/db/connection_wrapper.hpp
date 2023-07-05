#pragma once

#include "connection.hpp"

namespace fstore::core::db {
    class connection_wrapper : connection {
        std::shared_ptr<connection> inner;
    public:
        connection_wrapper(std::shared_ptr<connection>&& inner) :
            inner(std::forward<std::shared_ptr<connection>>(inner))
        {}

        auto add_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::string_view hash,
            std::int64_t size,
            std::string_view type,
            std::string_view subtype
        ) -> ext::task<object> override {
            return inner->add_object(
                bucket_id,
                object_id,
                hash,
                size,
                type,
                subtype
            );
        }

        auto create_bucket(
            std::string_view name
        ) -> ext::task<bucket> override {
            return inner->create_bucket(name);
        }

        auto fetch_bucket(
            std::string_view name
        ) -> ext::task<bucket> override {
            return inner->fetch_bucket(name);
        }

        auto fetch_buckets() ->
            ext::task<std::vector<bucket>> override
        {
            return inner->fetch_buckets();
        }

        auto fetch_buckets(
            std::span<const std::string> names
        ) -> ext::task<std::vector<bucket>> override {
            return inner->fetch_buckets(names);
        }

        auto fetch_store_totals() ->
            ext::task<store_totals> override
        {
            return inner->fetch_store_totals();
        }

        auto get_errors() ->
            ext::task<std::vector<object_error>> override
        {
            return inner->get_errors();
        }

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object> override {
            return inner->get_object(bucket_id, object_id);
        }

        auto get_objects(
            int batch_size
        ) -> ext::task<pg::portal<object>> override {
            return inner->get_objects(batch_size);
        }

        auto remove_bucket(
            const UUID::uuid& id
        ) -> ext::task<> override {
            return inner->remove_bucket(id);
        }

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object> override {
            return inner->remove_object(bucket_id, object_id);
        }

        auto remove_objects(
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<remove_result> override {
            return inner->remove_objects(bucket_id, objects);
        }

        auto remove_orphan_objects() ->
            ext::task<std::vector<object>> override
        {
            return inner->remove_orphan_objects();
        }

        auto rename_bucket(
            const UUID::uuid& id,
            std::string_view name
        ) -> ext::task<> override {
            return inner->rename_bucket(id, name);
        }

        auto update_object_errors(
            std::span<const object_error> records
        ) -> ext::task<> override {
            return inner->update_object_errors(records);
        }
    };
}
