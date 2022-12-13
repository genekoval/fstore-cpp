#include "../table.h"
#include "ObjectTest.test.h"

#include <fstore/core/db/database.env.test.h>

UUID::uuid ObjectTest::bucket_id;

auto ObjectTest::SetUpTestSuite() -> void {
    const auto bucket = fstore::test::DatabaseEnvironment::database()
        .create_bucket(bucket_name);

    bucket_id = bucket.id;
}

auto ObjectTest::TearDownTestSuite() -> void {
    fstore::test::DatabaseEnvironment::database()
        .remove_bucket(bucket_id);
}

auto ObjectTest::add_object(const fstore::core::db::object& object) -> void {
    database.add_object(
        bucket_id,
        object.id,
        object.hash,
        object.size,
        object.type,
        object.subtype
    );
}

auto ObjectTest::bucket() -> fstore::core::db::bucket {
    return database.fetch_bucket(bucket_name);
}

auto ObjectTest::bucket_size() -> std::size_t {
    return bucket().size;
}

auto ObjectTest::tables() -> std::vector<std::string> {
    return {
        fstore::test::table::object
    };
}
