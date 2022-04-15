#include "messages.h"
#include "object_store.test.h"

#include <fstore/error.h>

using testing::Return;

namespace {
    constexpr auto empty_bucket_name = std::string_view(
        fstore::core::messages::empty_bucket_name
    );
}

TEST_F(ObjectStoreTest, RenameBucket) {
    constexpr auto name = "foo"sv;

    EXPECT_CALL(database, rename_bucket(
        fstore::test::test_id,
        name
    )).WillOnce(Return());

    store.rename_bucket(fstore::test::test_id, name);
}

TEST_F(ObjectStoreTest, RenameBucketNameTrimmed) {
    constexpr auto name = " \t   foo   \n "sv;
    constexpr auto trimmed = "foo"sv;

    EXPECT_CALL(database, rename_bucket(
        fstore::test::test_id,
        trimmed
    )).WillOnce(Return());

    store.rename_bucket(fstore::test::test_id, name);
}

TEST_F(ObjectStoreTest, RenameBucketEmptyName) {
    try {
        store.rename_bucket(fstore::test::test_id, "");
        FAIL() << "Bucket renamed with empty name";
    }
    catch (const fstore::fstore_error& error) {
        ASSERT_EQ(empty_bucket_name, error.what());
    }
}

TEST_F(ObjectStoreTest, RenameBucketWhitespaceName) {
    try {
        store.rename_bucket(fstore::test::test_id, " \t   \n ");
        FAIL() << "Bucket renamed with whiltespace name";
    }
    catch (const fstore::fstore_error& error) {
        ASSERT_EQ(empty_bucket_name, error.what());
    }
}
