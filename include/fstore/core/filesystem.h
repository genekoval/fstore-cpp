#pragma once

#include <filesystem>
#include <fstream>
#include <netcore/fd.h>
#include <span>

namespace fstore::core::fs {
    class filesystem {
        const std::filesystem::path objects;
        const std::filesystem::path parts;

        auto make_object(
            std::string_view object_id,
            std::function<void(const std::filesystem::path&)>&& action
        ) const -> void;
    public:
        filesystem(const std::filesystem::path& home);

        auto copy(
            const std::filesystem::path& source,
            std::string_view id
        ) const -> void;

        auto get_part(std::string_view id) const -> std::ofstream;

        auto hash(std::span<const std::byte> buffer) const -> std::string;

        auto hash(const std::filesystem::path& path) const -> std::string;

        auto make_object(std::string_view part_id) -> void;

        auto mime_type(const std::filesystem::path& path) const -> std::string;

        auto object_path(std::string_view id) const -> std::filesystem::path;

        auto open(std::string_view id) const -> netcore::fd;

        auto part_path(std::string_view id) const -> std::filesystem::path;

        auto remove(std::string_view id) const -> void;

        auto remove_part(std::string_view id) const -> void;

        auto size(const std::filesystem::path& path) const -> uintmax_t;

        auto sync(
            std::string_view program,
            std::span<const std::string_view> options,
            std::string_view dest
        ) const -> void;
    };
}
