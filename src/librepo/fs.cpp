#include <crypto.h>

#include <fstore/repo.h>

namespace fstore::repo::fs {
    void copy_to_store(
        const std::filesystem::path& from,
        std::string_view to
    ) {
        std::filesystem::copy_file(
            from,
            to,
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
