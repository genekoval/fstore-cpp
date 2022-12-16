#include "FilesystemTest.test.hpp"

#include <fstream>

FilesystemTest::~FilesystemTest() {
    std::filesystem::remove_all(home);
}

auto FilesystemTest::SetUpTestSuite() -> void {
    auto file = std::ofstream(fstore::test::file::path);
    file << fstore::test::file::content << std::flush;
}
