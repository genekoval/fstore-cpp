#pragma once

#include <fstore/core/db/database.h>

#include <gtest/gtest.h>

namespace fstore::test {
    struct DatabaseEnvironment : testing::Environment {
        static auto connection() -> pqxx::connection&;

        static auto database() -> core::db::database&;

        virtual auto SetUp() -> void override;
    };
}
