#include "object_store.test.hpp"

#include <fstore/except.hpp>

using fstore::invalid_data;
using testing::Return;

TEST_F(ObjectStoreTest, CreateBucket) {
    const auto name = "foo";

    EXPECT_CALL(*db, create_bucket(std::string_view(name)))
        .WillOnce(Return(ext::make_task(
            fstore::core::db::bucket {.id = fstore::test::test_id, .name = name}
        )));

    const auto expected =
        fstore::bucket {.id = fstore::test::test_id, .name = name};

    [&]() -> ext::detached_task {
        const auto result = co_await store.create_bucket(name);
        EXPECT_EQ(expected, result);
    }();
}

TEST_F(ObjectStoreTest, CreateBucketNameTrimmed) {
    constexpr auto name = " \t  whitespace \n  "sv;
    constexpr auto trimmed = "whitespace"sv;

    EXPECT_CALL(*db, create_bucket(trimmed))
        .WillOnce(Return(ext::make_task(fstore::core::db::bucket {
            .id = fstore::test::test_id,
            .name = std::string(trimmed)})));

    const auto expected = fstore::bucket {
        .id = fstore::test::test_id,
        .name = std::string(trimmed)};

    [&]() -> ext::detached_task {
        const auto result = co_await store.create_bucket(name);
        EXPECT_EQ(expected, result);
    }();
}

TEST_F(ObjectStoreTest, CreateBucketEmptyName) {
    [&]() -> ext::detached_task {
        EXPECT_THROW(co_await store.create_bucket(""), invalid_data);
    }();
}

TEST_F(ObjectStoreTest, CreateBucketWhitespaceName) {
    [&]() -> ext::detached_task {
        EXPECT_THROW(co_await store.create_bucket("\t   \n"), invalid_data);
    }();
}
