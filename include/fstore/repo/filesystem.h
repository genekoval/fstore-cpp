#pragma once

#include <filesystem>
#include <netcore/fd.h>
#include <span>

namespace fstore::repo {
    class fs {
        const std::filesystem::path objects;
    public:
        fs(const std::filesystem::path& home);

        auto copy(
            const std::filesystem::path& source,
            std::string_view id
        ) const -> void;

        auto hash(std::span<const std::byte> buffer) const -> std::string;

        auto hash(const std::filesystem::path& path) const -> std::string;

        auto mime_type(std::span<const std::byte> buffer) const -> std::string;

        auto mime_type(const std::filesystem::path& path) const -> std::string;

        auto open(std::string_view id) const -> netcore::fd;

        auto path_to(std::string_view id) const -> std::filesystem::path;

        auto remove(std::string_view id) const -> void;

        auto size(const std::filesystem::path& path) const -> uintmax_t;

        auto write(
            std::string_view id,
            std::span<const std::byte> buffer
        ) const -> void;
    };
}
