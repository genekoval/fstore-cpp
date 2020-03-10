#pragma once

#include <filesystem>

namespace fstore::repo::fs {
    void copy_to_store(
        const std::filesystem::path& src,
        std::string_view object_id
    );

    std::string hash(const std::filesystem::path& path);

    std::string mime_type(const std::filesystem::path& path);

    void remove_from_store(std::string_view object_id);

    uintmax_t size(const std::filesystem::path& path);
}
