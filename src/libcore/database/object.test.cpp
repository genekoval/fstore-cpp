#include "database.test.h"

#include <fstore/error.h>

#include <uuid++/uuid.h>

TEST_F(ObjectTest, BucketAdditionRemoval) {
    const auto& object = objects.front();

    // The bucket should be initially empty.
    ASSERT_EQ(0, bucket_size());

    // Add the object to the bucket.
    add_object(object);
    ASSERT_EQ(1, bucket_size());

    // Adding the same object does nothing.
    for (auto i = 0u; i < 5u; i++) add_object(object);
    ASSERT_EQ(1, bucket_size());

    // Remove the object.
    database.remove_object(bucket_id, object.id);
    ASSERT_EQ(0, bucket_size());

    // The object we removed can be added again...
    add_object(object);
    ASSERT_EQ(1, bucket_size());

    // ...and removed again.
    database.remove_object(bucket_id, object.id);
    ASSERT_EQ(0, bucket_size());

    // Removing an object that does not exist results in an error.
    try {
        database.remove_object(bucket_id, object.id);
        FAIL() << "Removing a nonexistent object should have failed";
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ("bucket does not contain object"s, ex.what());
    }
}

TEST_F(ObjectTest, GetObjectNoObject) {
    auto opt = database.get_object(bucket_id, objects.front().id);
    ASSERT_FALSE(opt);
}

TEST_F(ObjectTest, GetObject) {
    const auto& object = objects.front();
    add_object(object);

    auto opt = database.get_object(bucket_id, object.id);
    ASSERT_TRUE(opt);

    const auto result = opt.value();

    ASSERT_EQ(object.id, result.id);
    ASSERT_EQ(object.hash, result.hash);
    ASSERT_EQ(object.size, result.size);
    ASSERT_EQ(object.type, result.type);
    ASSERT_EQ(object.subtype, result.subtype);
    ASSERT_FALSE(result.date_added.empty());
}

TEST_F(ObjectTest, RemoveObjectsEmptyList) {
    constexpr auto expected = fstore::core::db::remove_result {
        .objects_removed = 0,
        .space_freed = 0
    };

    const auto result = database.remove_objects(bucket_id, {});

    ASSERT_EQ(expected, result);
}

TEST_F(ObjectTest, RemoveObjectsSingle) {
    const auto& object = objects.front();
    const auto expected = fstore::core::db::remove_result {
        .objects_removed = 1,
        .space_freed = object.size
    };

    add_object(object);

    const auto result = database.remove_objects(bucket_id, {object.id});
    ASSERT_EQ(expected, result);
    ASSERT_EQ(0, bucket_size());
    ASSERT_EQ(1, count(fstore::test::table::object));
}

TEST_F(ObjectTest, RemoveObjectsMultiple) {
    auto size = 0ul;
    for (const auto& object : objects) {
        size += object.size;
        add_object(object);
    }

    const auto expected = fstore::core::db::remove_result {
        .objects_removed = objects.size(),
        .space_freed = size
    };

    auto ids = std::vector<UUID::uuid>();
    std::transform(
        objects.begin(),
        objects.end(),
        std::back_inserter(ids),
        [](const auto& object) -> UUID::uuid {
            return object.id;
        }
    );

    const auto result = database.remove_objects(bucket_id, ids);
    ASSERT_EQ(expected, result);
}

TEST_F(ObjectTest, RemoveObjectsSubset) {
    for (const auto& object : objects) add_object(object);
    database.remove_objects(bucket_id, {objects[0].id, objects[1].id});

    ASSERT_EQ(1, bucket_size());
    ASSERT_TRUE(database.get_object(bucket_id, objects[2].id));
}

TEST_F(ObjectTest, RemoveObjectsNonexistent) {
    constexpr auto expected = fstore::core::db::remove_result {
        .objects_removed = 0,
        .space_freed = 0
    };

    for (const auto& object : {objects[0], objects[1]}) add_object(object);

    const auto result = database.remove_objects(bucket_id, {objects[2].id});
    ASSERT_EQ(expected, result);

    ASSERT_EQ(2, bucket_size());
    for (auto i = 0; i <= 1; ++i) {
        ASSERT_TRUE(database.get_object(bucket_id, objects[i].id));
    }
}
