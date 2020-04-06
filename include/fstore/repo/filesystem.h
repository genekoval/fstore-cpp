#pragma once

#include <filesystem>
#include <memory>

namespace fstore::repo::fs {
    struct filesystem {
        virtual auto copy_object(
            const std::filesystem::path& source,
            std::string_view id
        ) -> void const = 0;

        virtual auto hash(
            const std::filesystem::path& path
        ) -> std::string const = 0;

        virtual auto mime_type(
            const std::filesystem::path& path
        ) -> std::string const = 0;

        virtual auto remove_object(std::string_view id) -> void const = 0;

        virtual auto size(
            const std::filesystem::path& path
        ) -> uintmax_t const = 0;
    };

    using fs_t = std::shared_ptr<filesystem>;

    auto get(const std::filesystem::path& objects) -> fs_t;
}
