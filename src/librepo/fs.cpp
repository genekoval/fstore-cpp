#include <crypto.h>
#include <magix.h>

#include <fstore/repo.h>

namespace fstore::repo::fs {
    const std::filesystem::path prefix(PREFIX);
    const std::filesystem::path data_dir(prefix / "var/lib/fstore");
    const std::filesystem::path object_dir(data_dir / "objects");

    void copy_to_store(
        const std::filesystem::path& from,
        std::string_view object_id
    ) {
        std::filesystem::create_directories(object_dir);

        std::filesystem::copy_file(
            from,
            object_dir / object_id,
            std::filesystem::copy_options::skip_existing
        );
    }

    std::string hash(const std::filesystem::path& path) {
        return crypto::sha256sum(path);
    }

    std::string mime_type(const std::filesystem::path& path) {
        magix::magic magic(MAGIC_MIME_TYPE);
        return magic.file(path);
    }

    void remove_from_store(std::string_view object_id) {
        std::filesystem::remove(object_dir / object_id);
    }

    uintmax_t size(const std::filesystem::path& path) {
        return std::filesystem::file_size(path);
    }
}
