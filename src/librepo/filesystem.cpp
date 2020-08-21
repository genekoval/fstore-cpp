#include "crypto.h"
#include "filesystem.h"

#include <fcntl.h>
#include <magix.h>

namespace fstore::repo::fs {
    auto get(const std::filesystem::path& objects) -> fs_t {
        return std::make_shared<default_fs>(objects);
    }

    default_fs::default_fs(const std::filesystem::path& objects) :
        objects(objects)
    {
        std::filesystem::create_directories(objects);
    }

    auto default_fs::copy_object(
        const std::filesystem::path& source,
        std::string_view id
    ) -> void const {
        std::filesystem::copy_file(
            source,
            objects / id,
            std::filesystem::copy_options::skip_existing
        );
    }

    auto default_fs::hash(
        const std::filesystem::path& path
    ) -> std::string const {
        return crypto::sha256sum(path);
    }

    auto default_fs::mime_type(
        const std::filesystem::path& path
    ) -> std::string const {
        return magix::magic(MAGIC_MIME_TYPE).file(path);
    }

    auto default_fs::open(std::string_view id) -> netcore::fd {
        auto path = objects/id;
        return ::open(path.c_str(), O_RDONLY);
    }

    auto default_fs::remove_object(std::string_view id) -> void const {
        std::filesystem::remove(objects / id);
    }

    auto default_fs::size(
        const std::filesystem::path& path
    ) -> uintmax_t const {
        return std::filesystem::file_size(path);
    }
}
