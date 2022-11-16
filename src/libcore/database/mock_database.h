#pragma once

#include <fstore/core/db/database.h>

#include <gmock/gmock.h>

namespace fstore::test {
    struct database : core::db::database {
        MOCK_METHOD(core::db::object, add_object, (
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::string_view hash,
            uintmax_t size,
            std::string_view type,
            std::string_view subtype
        ), (override));

        MOCK_METHOD(core::db::bucket, create_bucket, (
            const UUID::uuid& bucket_id,
            std::string_view name
        ), (override));

        MOCK_METHOD(core::db::bucket, fetch_bucket, (
            std::string_view name
        ), (override));

        MOCK_METHOD(std::vector<core::db::bucket>, fetch_buckets, (), (override));

        MOCK_METHOD(std::vector<core::db::bucket>, fetch_buckets, (
            const std::vector<std::string>& names
        ), (override));

        MOCK_METHOD(core::db::store_totals, fetch_store_totals, (), (override));

        MOCK_METHOD(std::optional<core::db::object>, get_object, (
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(ext::generator<std::span<core::db::object>>, get_objects, (
            int batch_size
        ), (override));

        MOCK_METHOD(void, remove_bucket, (std::string_view id), (override));

        MOCK_METHOD(core::db::object, remove_object, (
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ), (override));

        MOCK_METHOD(core::db::remove_result, remove_objects, (
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ), (override));

        MOCK_METHOD(std::vector<core::db::object>, remove_orphan_objects, (),
            (override));

        MOCK_METHOD(void, rename_bucket, (
            const UUID::uuid& id,
            std::string_view name
        ), (override));
    };
}
