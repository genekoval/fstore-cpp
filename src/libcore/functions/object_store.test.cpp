#include "object_store.test.hpp"

namespace {
    auto generate_uuid() -> UUID::uuid {
        return fstore::test::test_id;
    }
}

ObjectStoreTest::ObjectStoreTest() :
    store(database, filesystem, generate_uuid)
{}
