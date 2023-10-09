#include "BucketTest.test.hpp"
#include "../table.hpp"

#include <uuid++/uuid.h>

auto BucketTest::tables() -> std::vector<std::string_view> {
    return {fstore::test::table::bucket};
}
