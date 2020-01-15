#pragma once

#include <filesystem>
#include <string_view>

namespace service {
    void add_object(std::string_view bucket, const std::filesystem::path& path);
}
