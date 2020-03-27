#include <fstore/error.h>
#include <fstore/repo/object_store.h>

#include <gtest/gtest.h>

namespace db = fstore::repo::db;

using namespace std::literals;

class RepoBucketTest : public testing::Test {
protected:
    static auto TearDownTestSuite() -> void {
        db::object_store().truncate_buckets();
    }

    const db::object_store store;

    RepoBucketTest() : store() {
        store.truncate_buckets();
    }

    auto create_bucket(std::string_view name) -> db::bucket const {
        store.create_bucket(name);
        return store.fetch_bucket(name);
    }
};

TEST_F(RepoBucketTest, CreationDeletionWorks) {
    const auto name = "creation"s;

    auto bucket = create_bucket(name);
    ASSERT_EQ(name, bucket.name());

    bucket.drop();
    ASSERT_EQ(0, store.fetch_buckets().size());
}

TEST_F(RepoBucketTest, CreationWithEmptyNameFails) {
    auto test = [this](const std::string& name) {
        try {
            store.create_bucket(name);
            FAIL() << "Bucket created with name: " QUOTE(name);
        }
        catch (const fstore::fstore_error& ex) {
            ASSERT_EQ("cannot create bucket: bucket name empty"s, ex.what());
        }
    };

    test("");
    test(" ");
    test("              ");
}

TEST_F(RepoBucketTest, CreationWithNonUniqueNameFails) {
    const auto name = "unique"s;
    store.create_bucket(name);

    try {
        store.create_bucket(name);
        FAIL();
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ("cannot create bucket " QUOTE(name) ": bucket exists"s,
            ex.what());
    }
}

TEST_F(RepoBucketTest, FetchMultipleWorks) {
    const auto names = std::vector<std::string>{
        "one", "two", "three"
    };

    auto test = [&names](const auto buckets) {
        ASSERT_EQ(names.size(), buckets.size());

        auto contains = [&buckets](const std::string& name) -> bool {
            for (const auto& bucket : buckets)
                if (bucket.name() == name) return true;
            return false;
        };

        for (const auto& name : names)
            ASSERT_TRUE(contains(name));
    };

    for (const auto& bucket : names) store.create_bucket(bucket);

    auto buckets = store.fetch_buckets(names);
    test(buckets);
    buckets = store.fetch_buckets();
    test(buckets);
}

TEST_F(RepoBucketTest, RenameWithEmptyNameFails) {
    const auto original = "rename"s;
    auto bucket = create_bucket(original);

    auto test = [&bucket](const std::string& name) {
        try {
            bucket.name(name);
            FAIL() << "Bucket renamed with name: " QUOTE(name);
        }
        catch (const fstore::fstore_error& ex) {
            ASSERT_EQ("cannot rename bucket: name empty"s, ex.what());
        }
    };

    test("");
    test(" ");
    test("                   ");
}

TEST_F(RepoBucketTest, RenameWithNonUniqueNameFails) {
    const auto one = "one"s;
    auto bucket_one = create_bucket(one);

    // Passing in the same name should do nothing.
    bucket_one.name(one);
    ASSERT_EQ(one, bucket_one.name());

    const auto two = "two"s;
    auto bucket_two = create_bucket(two);

    try {
        bucket_two.name(one);
        FAIL() << "Renaming with another bucket's name should have failed";
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ(
            "cannot rename bucket: bucket named " QUOTE(one) " already exists"s,
            ex.what()
        );
    }
}

TEST_F(RepoBucketTest, RenameWorks) {
    const auto first = "first"s;
    auto bucket = create_bucket(first);

    const auto second = "second"s;
    bucket.name(second);

    ASSERT_EQ(second, bucket.name());
    ASSERT_EQ(bucket.id(), store.fetch_bucket(second).id());
}
