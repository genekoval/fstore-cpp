#pragma once

#include <fstore/core/db/database.h>

#include <gmock/gmock.h>

namespace fstore::test {
    struct database : core::db::database {
        MOCK_METHOD(core::db::object, add_object, (
            std::string_view bucket_id,
            std::string_view object_id,
            std::string_view hash,
            uintmax_t size,
            std::string mime_type
        ), (override));

        MOCK_METHOD(core::db::bucket, create_bucket, (
            std::string_view bucket_id,
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
            std::string_view bucket_id,
            std::string_view object_id
        ), (override));

        MOCK_METHOD(void, remove_bucket, (std::string_view id), (override));

        MOCK_METHOD(core::db::object, remove_object, (
            std::string_view bucket_id,
            std::string_view object_id
        ), (override));

        MOCK_METHOD(core::db::remove_result, remove_objects, (
            std::string_view bucket_id,
            const std::vector<std::string>& objects
        ), (override));

        MOCK_METHOD(std::vector<core::db::object>, remove_orphan_objects, (),
            (override));

        MOCK_METHOD(void, rename_bucket, (
            std::string_view id,
            std::string_view name
        ), (override));
    };
}
