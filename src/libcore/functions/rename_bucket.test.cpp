#include "object_store.test.hpp"

#include <fstore/except.hpp>

using fstore::invalid_data;
using testing::Return;

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
    EXPECT_THROW(store.rename_bucket(fstore::test::test_id, ""), invalid_data);
}

TEST_F(ObjectStoreTest, RenameBucketWhitespaceName) {
    EXPECT_THROW(
        store.rename_bucket(fstore::test::test_id, " \t   \n "),
        invalid_data
    );
}
