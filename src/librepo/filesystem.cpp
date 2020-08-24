#include "crypto.h"

#include <fstore/repo/filesystem.h>

#include <fcntl.h>
#include <magix.h>

namespace fstore::repo {
    constexpr auto object_dir = "objects";

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

    auto fs::hash(const std::filesystem::path& path) const -> std::string {
        return crypto::sha256sum(path);
    }

    auto fs::mime_type(const std::filesystem::path& path) const -> std::string {
        return magix::magic(MAGIC_MIME_TYPE).file(path);
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
}
