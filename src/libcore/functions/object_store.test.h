#pragma once

#include "../database/mock_database.h"
#include "../filesystem/mock_filesystem.h"

#include <fstore/core/object_store.h>

#include <gtest/gtest.h>

using namespace std::literals;

namespace fstore::test {
    const auto test_id = UUID::uuid("e4e1113b-b770-45ff-8903-645255c4d86d");
}

class ObjectStoreTest : public testing::Test {
protected:
    fstore::test::database database;
    fstore::test::filesystem filesystem;
    fstore::core::object_store store;

    ObjectStoreTest();
};
