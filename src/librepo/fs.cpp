#include <crypto.h>

#include <fstore/repo.h>

namespace fstore::repo::fs {
    std::string hash(const std::filesystem::path& path) {
        return crypto::sha256sum(path);
    }

    uintmax_t size(const std::filesystem::path& path) {
        return std::filesystem::file_size(path);
    }
}
