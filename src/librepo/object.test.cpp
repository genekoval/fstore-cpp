#include <fstore/error.h>
#include <fstore/test.h>

#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

class RepoObjectTest : public testing::Test {
protected:
    static constexpr auto bucket = "69595d19-8381-4c1f-ac52-52680df83675";
    static constexpr auto bucket_name = "repo-object-test";

    static auto SetUpTestSuite() -> void {
        fstore::test::db().create_bucket(bucket, bucket_name);
    }

    static auto TearDownTestSuite() -> void {
        fstore::test::drop_buckets();
        fstore::test::drop_objects();
    }

    fstore::repo::database db = fstore::test::db();
    const std::vector<fstore::repo::object> objects = {
        {
            .id = "a4f33eab-1fbf-49ab-af6b-9fc72714f8c5",
            .hash =
                "eab32d918fc1c07d87eddb59a4508666"
                "6f9117538d6d9c40ee0efeda635bd330",
            .size = 7,
            .mime_type = "text/plain"
        },
        {
            .id = "4ca61fdd-9058-4d08-a16b-744d3afc4b6d",
            .hash = "1c661c11465816c9bbcd70ac08a655cd"
                    "bf8dedf2122bd0ed1cb2d6b36b0e09ed",
            .size = 250,
            .mime_type = "text/plain"
        },
        {
            .id = "d7303d5a-3892-4888-a47e-b201f5219ee1",
            .hash = "ca3a6440be51f6a8687d25a683ff85c0"
                    "a9676d45b2f57f27b841b6b259893dc9",
            .size = 500,
            .mime_type = "text/plain"
        }
    };

    RepoObjectTest() {
        fstore::test::drop_objects();
    }
};

TEST_F(RepoObjectTest, BucketAdditionRemoval) {
    const auto& object = objects.front();

    auto count = [&]() -> std::size_t {
        return db.fetch_bucket(bucket_name).size;
    };

    // The bucket should be initially empty.
    ASSERT_EQ(0, count());

    // Add the object to the bucket.
    db.add_object(bucket, object);
    ASSERT_EQ(1, count());

    // Adding the same object does nothing.
    for (auto i = 0u; i < 5u; i++) db.add_object(bucket, object);
    ASSERT_EQ(1, count());

    // Remove the object.
    db.remove_object(bucket, object.id);
    ASSERT_EQ(0, count());

    // The object we removed can be added again...
    db.add_object(bucket, object);
    ASSERT_EQ(1, count());

    // ...and removed again.
    db.remove_object(bucket, object.id);
    ASSERT_EQ(0, count());

    // Removing an object that does not exist results in an error.
    try {
        db.remove_object(bucket, object.id);
        FAIL() << "Removing a nonexistent object should have failed";
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ("bucket does not contain object"s, ex.what());
    }
}

TEST_F(RepoObjectTest, GetObjectNoObject) {
    auto opt = db.get_object(bucket, objects.front().id);
    ASSERT_FALSE(opt);
}

TEST_F(RepoObjectTest, GetObject) {
    const auto& object = objects.front();
    db.add_object(bucket, object);

    auto opt = db.get_object(bucket, object.id);
    ASSERT_TRUE(opt);

    const auto result = opt.value();

    ASSERT_EQ(object.id, result.id);
    ASSERT_EQ(object.hash, result.hash);
    ASSERT_EQ(object.size, result.size);
    ASSERT_EQ(object.mime_type, result.mime_type);
    ASSERT_FALSE(result.date_added.empty());
}

TEST_F(RepoObjectTest, RemoveObjectsEmptyList) {
    constexpr auto expected = fstore::repo::remove_result {
        .objects_removed = 0,
        .space_freed = 0
    };

    const auto result = db.remove_objects(bucket, {});

    ASSERT_EQ(expected, result);
}

TEST_F(RepoObjectTest, RemoveObjectsSingle) {
    const auto& object = objects.front();
    const auto expected = fstore::repo::remove_result {
        .objects_removed = 1,
        .space_freed = object.size
    };

    db.add_object(bucket, object);

    const auto result = db.remove_objects(bucket, {object.id});
    ASSERT_EQ(expected, result);
    ASSERT_EQ(0, db.fetch_bucket(bucket_name).size);
    ASSERT_EQ(1, db.fetch_store_totals().objects);
}

TEST_F(RepoObjectTest, RemoveObjectsMultiple) {
    auto size = 0ul;
    for (const auto& object : objects) {
        size += object.size;
        db.add_object(bucket, object);
    }

    const auto expected = fstore::repo::remove_result {
        .objects_removed = objects.size(),
        .space_freed = size
    };

    auto ids = std::vector<std::string>();
    std::transform(
        objects.begin(),
        objects.end(),
        std::back_inserter(ids),
        [](const auto& object) -> std::string {
            return object.id;
        }
    );

    const auto result = db.remove_objects(bucket, ids);
    ASSERT_EQ(expected, result);
}

TEST_F(RepoObjectTest, RemoveObjectsSubset) {
    for (const auto& object : objects) db.add_object(bucket, object);
    db.remove_objects(bucket, {objects[0].id, objects[1].id});

    ASSERT_EQ(1, db.fetch_bucket(bucket_name).size);
    ASSERT_TRUE(db.get_object(bucket, objects[2].id));
}

TEST_F(RepoObjectTest, RemoveObjectsNonexistent) {
    constexpr auto expected = fstore::repo::remove_result {
        .objects_removed = 0,
        .space_freed = 0
    };

    for (const auto& object : {objects[0], objects[1]}) {
        db.add_object(bucket, object);
    }

    const auto result = db.remove_objects(bucket, {objects[2].id});
    ASSERT_EQ(expected, result);

    ASSERT_EQ(2, db.fetch_bucket(bucket_name).size);
    for (auto i = 0; i <= 1; ++i) {
        ASSERT_TRUE(db.get_object(bucket, objects[i].id));
    }
}
