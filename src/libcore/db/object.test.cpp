#include "database.test.hpp"

#include <fstore/except.hpp>

#include <unordered_set>
#include <uuid++/uuid.h>

using fstore::not_found;

namespace std {
    template <>
    struct hash<fstore::core::db::object> {
        auto operator()(
            const fstore::core::db::object& object
        ) const noexcept -> size_t {
            return hash<decltype(fstore::core::db::object::id)>{}(object.id);
        }
    };
}

TEST_F(ObjectTest, BucketAdditionRemoval) {
    run([&]() -> ext::task<> {
        const auto& object = objects.front();

        // The bucket should be initially empty.
        EXPECT_EQ(0, co_await bucket_size());

        // Add the object to the bucket.
        co_await add_object(object);
        EXPECT_EQ(1, co_await bucket_size());

        // Adding the same object does nothing.
        for (auto i = 0u; i < 5u; i++) co_await add_object(object);
        EXPECT_EQ(1, co_await bucket_size());

        // Remove the object.
        co_await connection->remove_object(bucket_id, object.id);
        EXPECT_EQ(0, co_await bucket_size());

        // The object we removed can be added again...
        co_await add_object(object);
        EXPECT_EQ(1, co_await bucket_size());

        // ...and removed again.
        co_await connection->remove_object(bucket_id, object.id);
        EXPECT_EQ(0, co_await bucket_size());

        // Removing an object that does not exist results in an error.
        EXPECT_THROW(
            co_await connection->remove_object(bucket_id, object.id),
            not_found
        );
    }());
}

TEST_F(ObjectTest, GetObjectNoObject) {
    run([&]() -> ext::task<> {
        EXPECT_THROW(
            co_await connection->get_object(bucket_id, objects.front().id),
            not_found
        );
    }());
}

TEST_F(ObjectTest, GetObject) {
    run([&]() -> ext::task<> {
        const auto& object = objects.front();
        co_await add_object(object);

        const auto result = co_await connection->get_object(
            bucket_id,
            object.id
        );

        EXPECT_EQ(object.id, result.id);
        EXPECT_EQ(object.hash, result.hash);
        EXPECT_EQ(object.size, result.size);
        EXPECT_EQ(object.type, result.type);
        EXPECT_EQ(object.subtype, result.subtype);
    }());
}

TEST_F(ObjectTest, GetObjects) {
    run([&]() -> ext::task<> {
        for (const auto& object : objects) co_await add_object(object);

        auto calls = 0;
        auto result_count = 0;
        auto objects_returned = std::unordered_set<fstore::core::db::object>();

        auto objects = co_await connection->get_objects(2);
        while (objects) {
            const auto results = co_await objects.next();

            ++calls;
            result_count += results.size();

            for (const auto& result : results) {
                objects_returned.insert(result);
            }
        }

        EXPECT_EQ(2, calls);
        EXPECT_EQ(3, result_count);
        EXPECT_EQ(3, objects_returned.size());
    }());
}

TEST_F(ObjectTest, RemoveObjectsEmptyList) {
    constexpr auto expected = fstore::remove_result {
        .objects_removed = 0,
        .space_freed = 0
    };

    run([&]() -> ext::task<> {
        const auto result = co_await connection->remove_objects(bucket_id, {});
        EXPECT_EQ(expected, result);
    }());
}

TEST_F(ObjectTest, RemoveObjectsSingle) {
    run([&]() -> ext::task<> {
        const auto& object = objects.front();
        const auto expected = fstore::remove_result {
            .objects_removed = 1,
            .space_freed = object.size
        };

        co_await add_object(object);

        const auto result = co_await connection->remove_objects(
            bucket_id,
            {object.id}
        );

        EXPECT_EQ(expected, result);
        EXPECT_EQ(0, co_await bucket_size());
        EXPECT_EQ(1, co_await count(fstore::test::table::object));
    }());
}

TEST_F(ObjectTest, RemoveObjectsMultiple) {
    run([&]() -> ext::task<> {
        auto size = 0ul;
        for (const auto& object : objects) {
            size += object.size;
            co_await add_object(object);
        }

        const auto expected = fstore::remove_result {
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

        const auto result = co_await connection->remove_objects(bucket_id, ids);
        EXPECT_EQ(expected, result);
    }());
}

TEST_F(ObjectTest, RemoveObjectsSubset) {
    run([&]() -> ext::task<> {
        for (const auto& object : objects) co_await add_object(object);
        co_await connection->remove_objects(
            bucket_id,
            {objects.at(0).id, objects.at(1).id}
        );

        EXPECT_EQ(1, co_await bucket_size());
        EXPECT_NO_THROW(
            co_await connection->get_object(bucket_id, objects[2].id)
        );
    }());
}

TEST_F(ObjectTest, RemoveObjectsNonexistent) {
    constexpr auto expected = fstore::remove_result {
        .objects_removed = 0,
        .space_freed = 0
    };

    run([&]() -> ext::task<> {
        for (const auto& object : {objects.at(0), objects.at(1)}) {
            co_await add_object(object);
        }

        const auto result = co_await connection->remove_objects(
            bucket_id,
            {objects.at(2).id}
        );
        EXPECT_EQ(expected, result);

        EXPECT_EQ(2, co_await bucket_size());

        for (auto i = 0; i <= 1; ++i) {
            EXPECT_NO_THROW(
                co_await connection->get_object(bucket_id, objects.at(i).id)
            );
        }
    }());
}
