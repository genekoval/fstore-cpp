#include "../table.h"
#include "ObjectTest.test.h"

#include <fstore/core/db/database.test.env.h>

const UUID::uuid ObjectTest::bucket_id = "69595d19-8381-4c1f-ac52-52680df83675";

auto ObjectTest::SetUpTestSuite() -> void {
    fstore::test::DatabaseEnvironment::database()
        .create_bucket(bucket_id, bucket_name);
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
