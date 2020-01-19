#pragma once

#include <filesystem>
#include <string>

namespace fstore::crypto {
    void sha256sum(std::string& checksum, const std::filesystem::path& path);
}
