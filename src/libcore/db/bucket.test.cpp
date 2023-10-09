#include "database.test.hpp"

#include <fstore/except.hpp>

#include <ext/string.h>
#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

TEST_F(BucketTest, Creation) {
    constexpr auto name = "creation"sv;

    run([&]() -> ext::task<> {
        const auto bucket = co_await connection->create_bucket(name);

        EXPECT_FALSE(bucket.id.is_null());
        EXPECT_EQ(name, bucket.name);
        EXPECT_EQ(0, bucket.size);
        EXPECT_EQ(0, bucket.space_used);
    }());
}

TEST_F(BucketTest, Removal) {
    run([&]() -> ext::task<> {
        const auto bucket = co_await connection->create_bucket("removal");
        EXPECT_EQ(1, co_await count("bucket"));

        co_await connection->remove_bucket(bucket.id);
        EXPECT_EQ(0, co_await count("bucket"));
    }());
}

TEST_F(BucketTest, CreationRequiresUniqueName) {
    constexpr auto name = "unique"sv;

    run([&]() -> ext::task<> {
        co_await connection->create_bucket(name);

        EXPECT_THROW(
            co_await connection->create_bucket(name),
            fstore::unique_bucket_violation
        );
    }());
}

TEST_F(BucketTest, FetchWorks) {
    run([&]() -> ext::task<> {
        const auto bucket = co_await connection->create_bucket("fetch");
        const auto fetched = co_await connection->fetch_bucket(bucket.name);

        EXPECT_EQ(bucket, fetched);
    }());
}

TEST_F(BucketTest, FetchMultipleWorks) {
    const auto names = std::vector<std::string> {"one", "two", "three"};

    auto test = [&names](const auto& buckets) -> void {
        EXPECT_EQ(names.size(), buckets.size());

        for (const auto& name : names) {
            EXPECT_TRUE(
                std::find_if(
                    buckets.begin(),
                    buckets.end(),
                    [&name](auto bucket) -> bool { return bucket.name == name; }
                ) != buckets.end()
            );
        }
    };

    run([&]() -> ext::task<> {
        for (const auto& name : names) {
            co_await connection->create_bucket(name);
        }

        test(co_await connection->fetch_buckets(names));
        test(co_await connection->fetch_buckets());
    }());
}

TEST_F(BucketTest, RenameWorks) {
    constexpr auto first = "first"sv;
    constexpr auto second = "second"sv;

    run([&]() -> ext::task<> {
        const auto id = (co_await connection->create_bucket(first)).id;
        co_await connection->rename_bucket(id, second);

        const auto bucket = co_await connection->fetch_bucket(second);

        EXPECT_EQ(id, bucket.id);
    }());
}

TEST_F(BucketTest, RenameWithNonUniqueNameFails) {
    constexpr auto one = "one"sv;
    constexpr auto two = "two"sv;

    run([&]() -> ext::task<> {
        auto first = co_await connection->create_bucket(one);
        auto fid = first.id;

        // Passing in the same name should do nothing.
        co_await connection->rename_bucket(fid, one);
        first = co_await connection->fetch_bucket(one);
        EXPECT_EQ(fid, first.id);

        auto second = co_await connection->create_bucket(two);

        EXPECT_THROW(
            co_await connection->rename_bucket(second.id, one),
            fstore::unique_bucket_violation
        );
    }());
}
