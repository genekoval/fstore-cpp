#include "crypto.h"

#include <fstore/repo/filesystem.h>

#include <fcntl.h>
#include <fstream>
#include <magix.h>

namespace fstore::repo {
    constexpr auto object_dir = "objects";
    constexpr auto parts_dir = "parts";

    static auto magic_mime_type() -> magix::magic {
        return magix::magic(MAGIC_MIME_TYPE);
    }

    fs::fs(const std::filesystem::path& home) :
        objects(home/object_dir),
        parts(home/parts_dir)
    {
        std::filesystem::create_directories(objects);
        std::filesystem::create_directories(parts);
    }

    auto fs::copy(
        const std::filesystem::path& source,
        std::string_view id
    ) const -> void {
        std::filesystem::copy_file(
            source,
            path_to(id),
            std::filesystem::copy_options::skip_existing
        );
    }

    auto fs::get_part(std::string_view id) const -> std::ofstream {
        auto path = parts/id;
        return std::ofstream(
            path,
            std::ios::out | std::ios::app | std::ios::binary
        );
    }

    auto fs::hash(std::span<const std::byte> buffer) const -> std::string {
        return crypto::sha256sum(buffer);
    }

    auto fs::hash(const std::filesystem::path& path) const -> std::string {
        return crypto::sha256sum(path);
    }

    auto fs::make_object(std::string_view part_id) -> void {
        auto part = parts/part_id;
        auto object = objects/part_id;

        std::filesystem::rename(part, object);
    }

    auto fs::mime_type(const std::filesystem::path& path) const -> std::string {
        return magic_mime_type().file(path);
    }

    auto fs::open(std::string_view id) const -> netcore::fd {
        auto path = path_to(id);
        return ::open(path.c_str(), O_RDONLY);
    }

    auto fs::part_path(std::string_view id) const -> std::filesystem::path {
        return parts/id;
    }

    auto fs::path_to(std::string_view id) const -> std::filesystem::path {
        return objects/id;
    }

    auto fs::remove(std::string_view id) const -> void {
        std::filesystem::remove(path_to(id));
    }

    auto fs::remove_part(std::string_view id) const -> void {
        std::filesystem::remove(parts/id);
    }

    auto fs::size(const std::filesystem::path& path) const -> uintmax_t {
        return std::filesystem::file_size(path);
    }
}
