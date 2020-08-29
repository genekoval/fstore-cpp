#include "crypto.h"

#include <fstore/repo/filesystem.h>

#include <fcntl.h>
#include <fstream>
#include <magix.h>

namespace fstore::repo {
    constexpr auto object_dir = "objects";

    static auto magic_mime_type() -> magix::magic {
        return magix::magic(MAGIC_MIME_TYPE);
    }

    fs::fs(const std::filesystem::path& home) : objects(home/object_dir) {
        std::filesystem::create_directories(objects);
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

    auto fs::hash(std::span<const std::byte> buffer) const -> std::string {
        return crypto::sha256sum(buffer);
    }

    auto fs::hash(const std::filesystem::path& path) const -> std::string {
        return crypto::sha256sum(path);
    }

    auto fs::mime_type(std::span<const std::byte> buffer) const -> std::string {
        return magic_mime_type().buffer(buffer);
    }

    auto fs::mime_type(const std::filesystem::path& path) const -> std::string {
        return magic_mime_type().file(path);
    }

    auto fs::open(std::string_view id) const -> netcore::fd {
        auto path = path_to(id);
        return ::open(path.c_str(), O_RDONLY);
    }

    auto fs::path_to(std::string_view id) const -> std::filesystem::path {
        return objects/id;
    }

    auto fs::remove(std::string_view id) const -> void {
        std::filesystem::remove(path_to(id));
    }

    auto fs::size(const std::filesystem::path& path) const -> uintmax_t {
        return std::filesystem::file_size(path);
    }

    auto fs::write(
        std::string_view id,
        std::span<const std::byte> buffer
    ) const -> void {
        const auto path = path_to(id);
        if (std::filesystem::exists(path)) return;

        const auto* data = reinterpret_cast<const char*>(buffer.data());

        auto file = std::ofstream(path, std::ios::out | std::ios::binary);
        file.write(data, buffer.size());
    }
}
