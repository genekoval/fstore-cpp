#pragma once

#include <fstore/repo/filesystem.h>

namespace fstore::repo::fs {
    class default_fs : public filesystem {
        std::filesystem::path objects;
    public:
        default_fs(const std::filesystem::path& objects);

        auto copy_object(
            const std::filesystem::path& source,
            std::string_view id
        ) -> void const override;

        auto hash(
            const std::filesystem::path& path
        ) -> std::string const override;

        auto mime_type(
            const std::filesystem::path& path
        ) -> std::string const override;

        auto open(std::string_view id) -> netcore::fd override;

        auto remove_object(std::string_view id) -> void const override;

        auto size(
            const std::filesystem::path& path
        ) -> uintmax_t const override;
    };
}
