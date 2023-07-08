#pragma once

#include "connection.hpp"

#include <gmock/gmock.h>

namespace fstore::test::db {
    struct mock_connection : core::db::connection {
        MOCK_METHOD(ext::task<core::db::object>, add_object, (
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::string_view hash,
            std::int64_t size,
            std::string_view type,
            std::string_view subtype
        ), (override));

        auto begin() -> ext::task<pg::transaction> override {
            co_return pg::transaction();
        }

        MOCK_METHOD(ext::task<core::db::bucket>, create_bucket, (
            std::string_view name
        ), (override));

        MOCK_METHOD(ext::task<core::db::bucket>, fetch_bucket, (
            std::string_view name
        ), (override));

        MOCK_METHOD(
            ext::task<std::vector<core::db::bucket>>,
            fetch_buckets, (), (override)
        );

        MOCK_METHOD(ext::task<std::vector<core::db::bucket>>, fetch_buckets, (
            std::span<const std::string> names
        ), (override));

        MOCK_METHOD(
            ext::task<core::db::store_totals>,
            fetch_store_totals, (), (override)
        );

        MOCK_METHOD(
            ext::task<std::vector<object_error>>, get_errors, (), (override)
        );

        MOCK_METHOD(ext::task<core::db::object>, get_object, (
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<pg::portal<core::db::object>>, get_objects, (
            std::string_view portal,
            int batch_size
        ), (override));

        MOCK_METHOD(ext::task<>, remove_bucket, (
            const UUID::uuid& id
        ), (override));

        MOCK_METHOD(ext::task<core::db::object>, remove_object, (
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::task<core::db::remove_result>, remove_objects, (
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ), (override));

        MOCK_METHOD(
            ext::task<std::vector<core::db::object>>,
            remove_orphan_objects, (), (override)
        );

        MOCK_METHOD(ext::task<>, rename_bucket, (
            const UUID::uuid& id,
            std::string_view name
        ), (override));

        MOCK_METHOD(ext::task<>, update_object_errors, (
            std::span<const object_error> records
        ), (override));
    };
}
