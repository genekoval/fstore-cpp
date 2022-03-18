#pragma once

#include <fstore/test.h>

#include <functional>
#include <filesystem>
#include <fstream>
#include <netcore/fd.h>
#include <span>

namespace fstore::core::fs {
    struct mime {
        std::string type;
        std::string subtype;
    };

    class filesystem {
        VIRTUAL auto create_directories(
            const std::filesystem::path& path
        ) const -> void;

        VIRTUAL auto make_object(
            std::string_view object_id,
            std::function<void(const std::filesystem::path&)>&& action
        ) const -> void;
    public:
        const std::filesystem::path objects;
        const std::filesystem::path parts;

        filesystem() = default;

        filesystem(const std::filesystem::path& home);

        VIRTUAL_DESTRUCTOR(filesystem);

        VIRTUAL auto copy(
            const std::filesystem::path& source,
            std::string_view id
        ) const -> void;

        VIRTUAL auto get_part(std::string_view id) const -> std::ofstream;

        VIRTUAL auto hash(
            std::span<const std::byte> buffer
        ) const -> std::string;

        VIRTUAL auto hash(
            const std::filesystem::path& path
        ) const -> std::string;

        VIRTUAL auto make_object(std::string_view part_id) const -> void;

        VIRTUAL auto mime_type(
            const std::filesystem::path& path
        ) const -> mime;

        VIRTUAL auto object_path(
            std::string_view id
        ) const -> std::filesystem::path;

        VIRTUAL auto open(std::string_view id) const -> netcore::fd;

        VIRTUAL auto part_path(
            std::string_view id
        ) const -> std::filesystem::path;

        VIRTUAL auto remove(std::string_view id) const -> void;

        VIRTUAL auto remove_part(std::string_view id) const -> void;

        VIRTUAL auto size(const std::filesystem::path& path) const -> uintmax_t;

        VIRTUAL auto sync(
            std::string_view program,
            std::span<const std::string_view> options,
            std::string_view dest
        ) const -> void;
    };
}
