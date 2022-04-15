#include "../table.h"
#include "BucketTest.test.h"

#include <uuid++/uuid.h>

auto BucketTest::create_bucket(
    std::string_view name
) -> fstore::core::db::bucket {
    return database.create_bucket(UUID::generate(), name);
}

auto BucketTest::tables() -> std::vector<std::string> {
    return {
        fstore::test::table::bucket
    };
}
