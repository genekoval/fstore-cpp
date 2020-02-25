#pragma once

#include <fstore/core.h>

#include <filesystem>
#include <string_view>
#include <vector>

namespace fstore::repo {
    namespace db {
        struct bucket {
            const std::string id;
            const std::string name;
            int object_count;
            uintmax_t space_used;
        };

        struct object {
            const std::string id;
            const std::string hash;
            const std::string mime_type;
            const uintmax_t size;
        };

        std::string add_object(
            std::string_view bucket_id,
            std::string_view object_id,
            std::string_view object_hash,
            uintmax_t object_size,
            std::string_view object_mime_type
        );

        void create_bucket(
            std::string_view bucket_id,
            std::string_view bucket_name
        );

        void delete_bucket(std::string_view bucket_id);

        std::vector<object> delete_orphan_objects();

        bucket fetch_bucket(std::string_view bucket_name);

        std::vector<bucket> fetch_buckets(
            const std::vector<std::string>& names
        );

        std::vector<bucket> fetch_buckets();

        core::store_totals get_store_totals();

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
            std::string_view object_id
        );

        std::string hash(const std::filesystem::path& path);

        std::string mime_type(const std::filesystem::path& path);

        void remove_from_store(std::string_view object_id);

        uintmax_t size(const std::filesystem::path& path);
    }
}
