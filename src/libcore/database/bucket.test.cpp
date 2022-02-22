#include <fstore/error.h>
#include <fstore/test.h>

#include <ext/string.h>
#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

class RepoBucketTest : public testing::Test {
protected:
    static auto TearDownTestSuite() -> void {
        fstore::test::drop_buckets();
    }

    fstore::core::db::database db;

    RepoBucketTest() : db(fstore::test::db()) {
        fstore::test::drop_buckets();
    }

    auto create_bucket(std::string_view name) -> fstore::core::db::bucket {
        return fstore::test::create_bucket(db, name);
    }
};

TEST_F(RepoBucketTest, CreationWorks) {
    auto uuid = UUID::uuid();
    uuid.generate();

    const auto id = uuid.string();
    const auto name = "creation"s;

    const auto bucket = db.create_bucket(id, name);

    ASSERT_EQ(id, bucket.id);
    ASSERT_EQ(name, bucket.name);
    ASSERT_EQ(0, bucket.size);
    ASSERT_EQ(0, bucket.space_used);
    ASSERT_FALSE(bucket.date_created.empty());
}

TEST_F(RepoBucketTest, RemovalWorks) {
    auto bucket = create_bucket("removal");
    ASSERT_EQ(1, db.fetch_store_totals().buckets);

    db.remove_bucket(bucket.id);
    ASSERT_EQ(0, db.fetch_store_totals().buckets);
}

/* TODO This test should be moved to 'core'.
TEST_F(RepoBucketTest, CreationWithEmptyNameFails) {
    auto test = [this](std::string_view name) {
        try {
            create_bucket(name);
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
*/

TEST_F(RepoBucketTest, CreationRequiresUniqueName) {
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

TEST_F(RepoBucketTest, FetchWorks) {
    auto bucket = create_bucket("fetch");
    auto fetched = db.fetch_bucket(bucket.name);

    ASSERT_EQ(bucket, fetched);
}

TEST_F(RepoBucketTest, FetchMultipleWorks) {
    const std::string name_array[] = {
        "one", "two", "three"
    };

    const auto names = std::vector<std::string>(
        std::begin(name_array),
        std::end(name_array)
    );

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

    test(db.fetch_buckets(names));
    test(db.fetch_buckets());
}

/* TODO This test should be moved to 'core'.
TEST_F(RepoBucketTest, RenameWithEmptyNameFails) {
    const auto original = "rename"s;
    auto bucket = create_bucket(original);

    auto test = [&bucket](const std::string& name) {
        try {
            bucket.name(name);
            FAIL() << "Bucket renamed with name: " + name;
        }
        catch (const fstore::fstore_error& ex) {
            ASSERT_EQ("cannot rename bucket: name empty"s, ex.what());
        }
    };

    test("");
    test(" ");
    test("                   ");
}
*/

TEST_F(RepoBucketTest, RenameWorks) {
    const auto first = "first"s;
    const auto second = "second"s;

    const auto id = create_bucket(first).id;
    db.rename_bucket(id, second);

    const auto bucket = db.fetch_bucket(second);

    ASSERT_EQ(id, bucket.id);
}

TEST_F(RepoBucketTest, RenameWithNonUniqueNameFails) {
    const auto one = "one"s;

    auto first = create_bucket(one);
    auto fid = first.id;

    // Passing in the same name should do nothing.
    db.rename_bucket(fid, one);
    first = db.fetch_bucket(one);
    ASSERT_EQ(fid, first.id);

    const auto two = "two"s;
    auto second = create_bucket(two);

    try {
        db.rename_bucket(second.id, one);
        FAIL() << "Renaming with another bucket's name should have failed";
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ(
            "cannot rename bucket: bucket named " QUOTE(one) " already exists"s,
            ex.what()
        );
    }
}
