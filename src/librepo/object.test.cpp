#include <fstore/error.h>
#include <fstore/test.h>

#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

class RepoObjectTest : public testing::Test {
protected:
    static auto TearDownTestSuite() -> void {
        fstore::test::drop_buckets();
        fstore::test::drop_objects();
    }

    fstore::repo::db db;
    fstore::model::object object;

    RepoObjectTest() :
        db(fstore::test::db()),
        object({
            .id = "a4f33eab-1fbf-49ab-af6b-9fc72714f8c5",
            .hash =
                "eab32d918fc1c07d87eddb59a4508666"
                "6f9117538d6d9c40ee0efeda635bd330",
            .size = 7,
            .mime_type = "text/plain"
        })
    {
        fstore::test::drop_buckets();
        fstore::test::drop_objects();
    }
};

TEST_F(RepoObjectTest, BucketAdditionRemoval) {
    // Create a bucket to add objects to.
    auto bucket = fstore::test::create_bucket(db, "test");

    // Asserts that the bucket contains the specified number of objects.
    auto assert_count = [this, &bucket](int count) {
        // Fetch the bucket again to ensure information
        // is read from the database.
        bucket = db.fetch_bucket(bucket.name);
        ASSERT_EQ(count, bucket.size);
    };

    // The bucket should be initially empty.
    assert_count(0);

    // Add the object to the bucket.
    db.add_object(bucket.id, object);
    assert_count(1);

    // Adding the same object does nothing.
    for (auto i = 0u; i < 5u; i++) db.add_object(bucket.id, object);
    assert_count(1);

    // Remove the object.
    db.remove_object(bucket.id, object.id);
    assert_count(0);

    // The object we removed can be added again...
    db.add_object(bucket.id, object);
    assert_count(1);

    // ...and removed again.
    db.remove_object(bucket.id, object.id);
    assert_count(0);

    // Removing an object that does not exist results in an error.
    try {
        db.remove_object(bucket.id, object.id);
        FAIL() << "Removing a nonexistent object should have failed";
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ("bucket does not contain object"s, ex.what());
    }
}

TEST_F(RepoObjectTest, GetObjectNoBucket) {
    auto uuid = UUID::uuid();
    uuid.generate();

    auto opt = db.get_object(uuid.string(), object.id);

    ASSERT_FALSE(opt.has_value());
}

TEST_F(RepoObjectTest, GetObjectNoObject) {
    auto bucket = fstore::test::create_bucket(db, "test");

    auto opt = db.get_object(bucket.id, object.id);

    ASSERT_FALSE(opt.has_value());
}

TEST_F(RepoObjectTest, GetObject) {
    auto bucket = fstore::test::create_bucket(db, "test");
    db.add_object(bucket.id, object);

    auto opt = db.get_object(bucket.id, object.id);
    ASSERT_TRUE(opt.has_value());

    auto result = opt.value();

    ASSERT_EQ(object.id, result.id);
    ASSERT_EQ(object.hash, result.hash);
    ASSERT_EQ(object.size, result.size);
    ASSERT_EQ(object.mime_type, result.mime_type);
    ASSERT_EQ(object.date_added, result.date_added);
}
