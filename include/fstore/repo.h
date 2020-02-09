#pragma once

#include <filesystem>
#include <string_view>

namespace fstore::repo {
    namespace db {
        struct object {
            std::string id;
            std::string hash;
            uintmax_t size;
        };

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

        object remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        );

        void rename_bucket(
            std::string_view bucket_id,
            std::string_view new_name
        );
    }

    namespace fs {
        void copy_to_store(
            const std::filesystem::path& src,
            const std::filesystem::path& dest_filename
        );

        std::string hash(const std::filesystem::path& path);

        uintmax_t size(const std::filesystem::path& path);
    }
}
