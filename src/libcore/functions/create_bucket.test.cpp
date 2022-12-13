#include "messages.h"
#include "object_store.test.h"

#include <fstore/error.h>

using testing::Return;

namespace {
    constexpr auto empty_bucket_name = std::string_view(
        fstore::core::messages::empty_bucket_name
    );
}

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
    try {
        store.create_bucket("");
        FAIL() << "Bucket created with empty name";
    }
    catch (const fstore::fstore_error& error) {
        ASSERT_EQ(empty_bucket_name, error.what());
    }
}

TEST_F(ObjectStoreTest, CreateBucketWhitespaceName) {
    try {
        store.create_bucket("\t   \n");
        FAIL() << "Bucket created with whitespace name";
    }
    catch (const fstore::fstore_error& error) {
        ASSERT_EQ(empty_bucket_name, error.what());
    }
}
