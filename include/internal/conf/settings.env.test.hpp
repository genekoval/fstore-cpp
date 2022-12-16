#pragma once

#include "settings.hpp"

#include <gtest/gtest.h>

namespace fstore::test {
    struct SettingsEnvironment: testing::Environment {
        static auto settings() -> const conf::settings&;

        virtual auto SetUp() -> void override;
    };
}
