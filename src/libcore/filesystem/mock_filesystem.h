#pragma once

#include <fstore/core/filesystem.h>

#include <gmock/gmock.h>

namespace fstore::test {
    struct filesystem : core::fs::filesystem {
        MOCK_METHOD(void, copy, (
            const std::filesystem::path& source,
            std::string_view id
        ), (const, override));

        MOCK_METHOD(std::ofstream, get_part, (
            std::string_view id
        ), (const, override));

        MOCK_METHOD(std::string, hash, (
            std::span<const std::byte> buffer
        ), (const, override));

        MOCK_METHOD(std::string, hash, (
            const std::filesystem::path& path
        ), (const, override));

        MOCK_METHOD(void, make_object, (
            std::string_view part_id
        ), (const, override));

        MOCK_METHOD(std::string, mime_type, (
            const std::filesystem::path& path
        ), (const, override));

        MOCK_METHOD(std::filesystem::path, object_path, (
            std::string_view id
        ), (const, override));

        MOCK_METHOD(netcore::fd, open, (
            std::string_view id
        ), (const, override));

        MOCK_METHOD(std::filesystem::path, part_path, (
            std::string_view id
        ), (const, override));

        MOCK_METHOD(void, remove, (std::string_view id), (const, override));

        MOCK_METHOD(void, remove_part, (
            std::string_view id
        ), (const, override));

        MOCK_METHOD(uintmax_t, size, (
            const std::filesystem::path& path
        ), (const, override));

        MOCK_METHOD(void, sync, (
            std::string_view program,
            std::span<const std::string_view> options,
            std::string_view dest
        ), (const, override));
    };
}
