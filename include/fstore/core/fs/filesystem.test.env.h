#pragma once

#include <filesystem>
#include <gtest/gtest.h>

namespace fstore::test {
    struct FilesystemEnvironment : testing::Environment {
        static auto path() -> std::filesystem::path;

        virtual auto SetUp() -> void override;
    };
}
