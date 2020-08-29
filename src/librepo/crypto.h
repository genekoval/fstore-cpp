#pragma once

#include <filesystem>
#include <span>
#include <string>

namespace fstore::crypto {
    auto sha256sum(std::span<const std::byte> buffer) -> std::string;

    auto sha256sum(const std::filesystem::path& path) -> std::string;
}
