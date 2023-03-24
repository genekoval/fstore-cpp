#include "../table.hpp"
#include "ObjectTest.test.hpp"

UUID::uuid ObjectTest::bucket_id;

auto ObjectTest::SetUpTestSuite() -> void {
    netcore::run([]() -> ext::task<> {
        auto db = database();
        auto connection = co_await db.connect();

        const auto bucket = co_await connection.create_bucket(bucket_name);
        bucket_id = bucket.id;
    }());
}

auto ObjectTest::TearDownTestSuite() -> void {
    netcore::run([]() -> ext::task<> {
        auto db = database();
        auto connection = co_await db.connect();

        co_await connection.remove_bucket(bucket_id);
    }());
}

auto ObjectTest::add_object(const fstore::object& object) -> ext::task<> {
    co_await connection->add_object(
        bucket_id,
        object.id,
        object.hash,
        object.size,
        object.type,
        object.subtype
    );
}

auto ObjectTest::bucket() -> ext::task<fstore::bucket> {
    co_return co_await connection->fetch_bucket(bucket_name);
}

auto ObjectTest::bucket_size() -> ext::task<std::size_t> {
    co_return (co_await bucket()).size;
}

auto ObjectTest::tables() -> std::vector<std::string_view> {
    return {
        fstore::test::table::object
    };
}
