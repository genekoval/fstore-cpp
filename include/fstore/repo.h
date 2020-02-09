#pragma once

#include <filesystem>
#include <string_view>

namespace fstore::repo {
    namespace db {
        std::string add_object(
            std::string_view bucket_id,
            std::string_view object_id,
            std::string_view object_hash,
            uintmax_t object_size
        );

        void create_bucket(
            std::string_view bucket_id,
            std::string_view bucket_name
        );

        void delete_bucket(std::string_view bucket_id);

        std::string fetch_bucket(std::string_view bucket_name);

        void rename_bucket(
            std::string_view bucket_id,
            std::string_view new_name
        );
    }

    namespace fs {
        void copy_to_store(
            const std::filesystem::path& from,
            std::string_view to
        );

        std::string hash(const std::filesystem::path& path);

        uintmax_t size(const std::filesystem::path& path);
    }
}
