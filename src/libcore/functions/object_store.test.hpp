#pragma once

#include <internal/core/db/database.test.hpp>
#include <internal/core/db/mock_connection.test.hpp>
#include <internal/core/fs/mock_filesystem.hpp>
#include <internal/core/object_store.hpp>

#include <gtest/gtest.h>

using namespace std::literals;

namespace fstore::test {
    const UUID::uuid test_id = "e4e1113b-b770-45ff-8903-645255c4d86d";
}

class ObjectStoreTest : public testing::Test {
protected:
    std::shared_ptr<fstore::test::db::mock_connection> db;
    fstore::test::db::database database;
    fstore::test::filesystem filesystem;
    fstore::core::object_store store;

    ObjectStoreTest();
};
