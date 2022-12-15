#include "object_store.test.h"

#include <fstore/error.h>

using fstore::invalid_data;
using testing::Return;

TEST_F(ObjectStoreTest, CreateBucket) {
    const auto name = "foo";

    EXPECT_CALL(database, create_bucket(
        std::string_view(name)
    )).WillOnce(Return(fstore::core::db::bucket {
        .id = fstore::test::test_id,
        .name = name
    }));

    const auto expected = fstore::core::bucket {
        .id = fstore::test::test_id,
        .name = name
    };
    const auto result = store.create_bucket(name);

    ASSERT_EQ(expected, result);
}

TEST_F(ObjectStoreTest, CreateBucketNameTrimmed) {
    constexpr auto name = " \t  whitespace \n  "sv;
    constexpr auto trimmed = "whitespace"sv;

    EXPECT_CALL(database, create_bucket(
        trimmed
    )).WillOnce(Return(fstore::core::db::bucket {
        .id = fstore::test::test_id,
        .name = std::string(trimmed)
    }));

    const auto expected = fstore::core::bucket {
        .id = fstore::test::test_id,
        .name = std::string(trimmed)
    };
    const auto result = store.create_bucket(name);

    ASSERT_EQ(expected, result);
}

TEST_F(ObjectStoreTest, CreateBucketEmptyName) {
    EXPECT_THROW(store.create_bucket(""), invalid_data);
}

TEST_F(ObjectStoreTest, CreateBucketWhitespaceName) {
    EXPECT_THROW(store.create_bucket("\t   \n"), invalid_data);
}
