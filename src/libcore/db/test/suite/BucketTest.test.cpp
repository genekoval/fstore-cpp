#include "../table.hpp"
#include "BucketTest.test.hpp"

#include <uuid++/uuid.h>

auto BucketTest::tables() -> std::vector<std::string> {
    return {
        fstore::test::table::bucket
    };
}
