#pragma once

#include <fstore/repo/object_store.h>

namespace fstore::test {
    auto test_store() -> repo::db::object_store;
}
