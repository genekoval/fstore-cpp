#pragma once

#include "settings.hpp"

#include <gtest/gtest.h>
#include <pg++/pg++>

namespace fstore::test {
    struct SettingsEnvironment: testing::Environment {
        static auto db_params() -> const pg::parameters&;

        static auto settings() -> const conf::settings&;

        virtual auto SetUp() -> void override;
    };
}
