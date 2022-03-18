#include "filesystem.test.h"

#include <fstore/core/filesystem.h>

#include <fstream>
#include <gtest/gtest.h>
#include <uuid++/uuid.h>

TEST_F(FilesystemTest, CopyObject) {
    constexpr auto id = "37daf6fc-bacc-47a7-9298-dd40f80d1b90";

    filesystem.copy(fstore::test::file::path, id);

    const auto path = filesystem.object_path(id);

    ASSERT_TRUE(std::filesystem::exists(path));
    ASSERT_EQ(filesystem.objects / "37" / "da" / id, path);

    auto file = std::ifstream(path);
    auto content = std::string();
    file >> content;

    ASSERT_EQ(fstore::test::file::content, content);
}

TEST_F(FilesystemTest, Hash) {
    ASSERT_EQ(
        fstore::test::file::hash,
        filesystem.hash(fstore::test::file::path)
    );
}

TEST_F(FilesystemTest, MimeType) {
    const auto mime = filesystem.mime_type(fstore::test::file::path);

    ASSERT_EQ(fstore::test::file::type, mime.type);
    ASSERT_EQ(fstore::test::file::subtype, mime.subtype);
}

TEST_F(FilesystemTest, Size) {
    ASSERT_EQ(
        fstore::test::file::size,
        filesystem.size(fstore::test::file::path)
    );
}
