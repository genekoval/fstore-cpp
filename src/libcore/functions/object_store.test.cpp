#include "object_store.test.hpp"

namespace {
    auto generate_uuid() -> UUID::uuid { return fstore::test::test_id; }
}

ObjectStoreTest::ObjectStoreTest() :
    db(new fstore::test::db::mock_connection()),
    database(std::shared_ptr<fstore::test::db::mock_connection>(db)),
    store(database, filesystem, generate_uuid) {}
