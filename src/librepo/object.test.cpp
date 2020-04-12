#include "repo.h"

#include <fstore/error.h>

#include <gtest/gtest.h>

namespace core = fstore::core;
namespace db = fstore::repo::db;

using namespace std::literals;

using fstore::test::test_store;

class RepoObjectTest : public testing::Test {
protected:
    /**
     * A simple aggregate for holding data for test objects.
     */
    struct data {
        /**
         * Returns metadata from the following text:
         *      object\n
         * returns: Metadata for some text data.
         */
        static auto text() -> data {
            return data {
                "eab32d918fc1c07d87eddb59a4508666"
                "6f9117538d6d9c40ee0efeda635bd330",
                "text/plain",
                7
            };
        }

        const std::string hash;
        const std::string mime_type;
        const uintmax_t size;
    };

    static auto TearDownTestSuite() -> void {
        auto store = test_store();
        store.truncate_buckets();
        store.truncate_objects();
    }

    db::object_store store;

    RepoObjectTest() : store(test_store()) {
        store.truncate_buckets();
        store.truncate_objects();
    }
};

TEST_F(RepoObjectTest, Creation) {
    const auto text = data::text();
    auto object = db::object(
        store.connection(),
        text.hash,
        text.mime_type,
        text.size
    );

    ASSERT_EQ(text.hash, object.hash());
    ASSERT_EQ(text.mime_type, object.mime_type());
    ASSERT_EQ(text.size, object.size());

    // Creating an object with the same hash should result in the same
    // object instance.
    // TODO Should the mime type and size also have to match?
    auto another = db::object(
        store.connection(),
        text.hash,
        text.mime_type,
        text.size
    );

    // TODO Overload equality operator for objects.
    ASSERT_EQ(object.id(), another.id());
}

TEST_F(RepoObjectTest, BucketAdditionRemoval) {
    // Create a test object.
    const auto text = data::text();
    std::unique_ptr<core::object> object = std::make_unique<db::object>(
        store.connection(),
        text.hash,
        text.mime_type,
        text.size
    );

    // Create a bucket to add the object to.
    const auto name = "test";
    store.create_bucket(name);
    auto bucket = store.fetch_bucket(name);
    ASSERT_EQ(0, bucket.object_count());

    // Asserts that the bucket contains the specified number of objects.
    auto assert_count = [this, &name](int count) {
        // Fetch the bucket again to ensure information
        // is read from the database.
        ASSERT_EQ(count, store.fetch_bucket(name).object_count());
    };

    // Add the object to the bucket.
    bucket.add(object);
    assert_count(1);

    // Adding the same object does nothing.
    for (auto i = 0u; i < 5u; i++) bucket.add(object);
    assert_count(1);

    // Remove the object.
    bucket.remove(object->id());
    assert_count(0);

    // The object we removed can be added again...
    bucket.add(object);
    assert_count(1);

    // ...and removed again.
    bucket.remove(object->id());
    assert_count(0);

    // Removing an object that does not exist results in an error.
    try {
        bucket.remove(object->id());
        FAIL() << "Removing a nonexistent object should have failed";
    }
    catch (const fstore::fstore_error& ex) {
        ASSERT_EQ("bucket does not contain object"s, ex.what());
    }
}
