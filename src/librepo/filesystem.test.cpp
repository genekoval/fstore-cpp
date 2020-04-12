#include <filesystem.h>

#include <gtest/gtest.h>
#include <stdlib.h>

#include <fstream>
#include <uuid++/uuid.h>

using namespace std::literals;

constexpr const char* dir_template = "fstore.test.XXXXXX";

static auto create_temp_directory() -> std::filesystem::path {
    auto path = std::filesystem::temp_directory_path() / dir_template;
    auto str = path.string();
    auto size = str.size();

    auto dir = new char[size];
    str.copy(dir, size);

    path = mkdtemp(dir);

    delete[] dir;
    return path;
}

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
        const auto path = directory / content;

        auto file = std::ofstream(path);
        file << content << std::flush;

        return path;
    }
}

class RepoFilesystemTest : public testing::Test {
protected:
    const std::filesystem::path app_dir;
    const fstore::repo::fs::fs_t fs;
    const std::filesystem::path test_file_path;

    RepoFilesystemTest() :
        app_dir(create_temp_directory()),
        fs(fstore::repo::fs::get(app_dir / "objects")),
        test_file_path(test_file::write(app_dir))
    {}

    ~RepoFilesystemTest() {
        std::filesystem::remove_all(app_dir);
    }
};

TEST_F(RepoFilesystemTest, CopyObject) {
    auto uuid = UUID::uuid();
    uuid.generate();

    fs->copy_object(test_file_path, uuid.string());

    auto object = app_dir / "objects" / uuid.string();

    ASSERT_TRUE(std::filesystem::exists(object));

    auto object_file = std::ifstream(object);
    auto object_content = std::string();
    object_file >> object_content;

    ASSERT_EQ(test_file::content, object_content);
}

TEST_F(RepoFilesystemTest, Hash) {
    ASSERT_EQ(test_file::hash, fs->hash(test_file_path));
}

TEST_F(RepoFilesystemTest, MimeType) {
    ASSERT_EQ(test_file::type, fs->mime_type(test_file_path));
}

TEST_F(RepoFilesystemTest, Size) {
    ASSERT_EQ(test_file::size, fs->size(test_file_path));
}