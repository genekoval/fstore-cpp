#pragma once

#include <filesystem>
#include <string>

namespace fstore::crypto {
    std::string sha256sum(const std::filesystem::path& path);
}
