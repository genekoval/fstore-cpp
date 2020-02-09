#include <crypto.h>

#include <fstore/repo.h>

namespace fstore::repo::fs {
    const std::filesystem::path prefix(PREFIX);
    const std::filesystem::path data_dir(prefix / "var/lib/fstore");
    const std::filesystem::path object_dir(data_dir / "objects");

    void copy_to_store(
        const std::filesystem::path& from,
        const std::filesystem::path& dest_filename
    ) {
        std::filesystem::create_directories(object_dir);

        std::filesystem::copy_file(
            from,
            object_dir / dest_filename,
            std::filesystem::copy_options::skip_existing
        );
    }

    std::string hash(const std::filesystem::path& path) {
        return crypto::sha256sum(path);
    }

    uintmax_t size(const std::filesystem::path& path) {
        return std::filesystem::file_size(path);
    }
}
