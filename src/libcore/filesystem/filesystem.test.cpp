#include <fstore/core/filesystem.h>
#include <fstore/test.h>

#include <fstream>
#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

namespace test_file {
    static constexpr auto content = "MyObjectFile";
    static constexpr auto hash = // SHA-256 sum of file content
        "348fa060a89f766f80d703887208fe66"
        "278712f1e2329bda348a4fc3a3abe617"sv;
    static constexpr auto size = 12;
    static constexpr auto type = "text/plain"sv;

    static auto write(
        const std::filesystem::path& directory
    ) -> std::filesystem::path {
        const auto path = directory/content;

        auto file = std::ofstream(path);
        file << content << std::flush;

        return path;
    }
}

class RepoFilesystemTest : public testing::Test {
protected:
    const fstore::test::temp_directory home;
    const fstore::core::filesystem fs;
    const std::filesystem::path test_file_path;

    RepoFilesystemTest() :
        fs(home.path),
        test_file_path(test_file::write(home.path))
    {}
};

TEST_F(RepoFilesystemTest, CopyObject) {
    auto uuid = UUID::uuid();
    uuid.generate();

    auto id = uuid.string();

    fs.copy(test_file_path, id);

    auto object = fs.path_to(id);

    ASSERT_TRUE(std::filesystem::exists(object));

    auto object_file = std::ifstream(object);
    auto object_content = std::string();
    object_file >> object_content;

    ASSERT_EQ(test_file::content, object_content);
}

TEST_F(RepoFilesystemTest, Hash) {
    ASSERT_EQ(test_file::hash, fs.hash(test_file_path));
}

TEST_F(RepoFilesystemTest, MimeType) {
    ASSERT_EQ(test_file::type, fs.mime_type(test_file_path));
}

TEST_F(RepoFilesystemTest, Size) {
    ASSERT_EQ(test_file::size, fs.size(test_file_path));
}
