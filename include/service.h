#pragma once

#include <filesystem>
#include <string>

namespace service {
    void add_object(const std::string& bucket, const std::filesystem::path& path);
}
