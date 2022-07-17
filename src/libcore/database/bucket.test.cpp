#include "database.test.h"

#include <fstore/error.h>

#include <ext/string.h>
#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

TEST_F(BucketTest, Creation) {
    const auto id = UUID::generate();
    const auto name = "creation"s;

    const auto bucket = database.create_bucket(id, name);

    EXPECT_EQ(id, bucket.id);
    EXPECT_EQ(name, bucket.name);
    EXPECT_EQ(0, bucket.size);
    EXPECT_EQ(0, bucket.space_used);
}

TEST_F(BucketTest, Removal) {
    const auto bucket = create_bucket("removal");
    ASSERT_EQ(1, count("bucket"));

    database.remove_bucket(bucket.id);
    ASSERT_EQ(0, count("bucket"));
}

TEST_F(BucketTest, CreationRequiresUniqueName) {
    const auto name = "unique"s;
    create_bucket(name);

    try {
        create_bucket(name);
        FAIL();
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ(
            "cannot create bucket " QUOTE(name) ": bucket exists"s,
            ex.what()
        );
    }
}

TEST_F(BucketTest, FetchWorks) {
    const auto bucket = create_bucket("fetch");
    const auto fetched = database.fetch_bucket(bucket.name);

    ASSERT_EQ(bucket, fetched);
}

TEST_F(BucketTest, FetchMultipleWorks) {
    const auto names = std::vector<std::string> {
        "one", "two", "three"
    };

    auto test = [&names](const auto& buckets) {
        ASSERT_EQ(names.size(), buckets.size());

        for (const auto& name : names) {
            ASSERT_TRUE(std::find_if(
                buckets.begin(),
                buckets.end(),
                [&name](auto bucket) -> bool {
                    return bucket.name == name;
                }
            ) != buckets.end());
        }
    };

    for (const auto& name: names) create_bucket(name);

    test(database.fetch_buckets(names));
    test(database.fetch_buckets());
}

TEST_F(BucketTest, RenameWorks) {
    const auto first = "first"s;
    const auto second = "second"s;

    const auto id = create_bucket(first).id;
    database.rename_bucket(id, second);

    const auto bucket = database.fetch_bucket(second);

    ASSERT_EQ(id, bucket.id);
}

TEST_F(BucketTest, RenameWithNonUniqueNameFails) {
    const auto one = "one"s;

    auto first = create_bucket(one);
    auto fid = first.id;

    // Passing in the same name should do nothing.
    database.rename_bucket(fid, one);
    first = database.fetch_bucket(one);
    ASSERT_EQ(fid, first.id);

    const auto two = "two"s;
    auto second = create_bucket(two);

    try {
        database.rename_bucket(second.id, one);
        FAIL() << "Renaming with another bucket's name should have failed";
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ(
            "cannot rename bucket: bucket named " QUOTE(one) " already exists"s,
            ex.what()
        );
    }
}
