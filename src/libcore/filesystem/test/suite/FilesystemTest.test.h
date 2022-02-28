#pragma once

#include <fstore/core/fs/filesystem.test.env.h>
#include <fstore/core/filesystem.h>

namespace fstore::test::file {
    const std::string content = "MyObjectFile";
    const std::string hash = // SHA-256 sum of file content
        "348fa060a89f766f80d703887208fe66"
        "278712f1e2329bda348a4fc3a3abe617";
    const std::filesystem::path path =
        FilesystemEnvironment::path() / "test_file";
    const std::size_t size = 12;
    const std::string type = "text/plain";
}

class FilesystemTest : public testing::Test {
    const std::filesystem::path home =
        fstore::test::FilesystemEnvironment::path() / "home";
protected:
    static auto SetUpTestSuite() -> void;

    ~FilesystemTest();

    fstore::core::fs::filesystem filesystem =
        fstore::core::fs::filesystem(home);
};
