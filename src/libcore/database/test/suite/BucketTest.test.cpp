#include "../table.h"
#include "BucketTest.test.h"

#include <uuid++/uuid.h>

auto BucketTest::tables() -> std::vector<std::string> {
    return {
        fstore::test::table::bucket
    };
}
