#pragma once

#include <fstore/repo/bucket.h>
#include <fstore/repo/object.h>
#include <fstore/repo/store_totals.h>

#include <string_view>

namespace fstore::repo::db {
    struct object_store {
        void create_bucket(std::string_view bucket_name) const;

        std::string create_object(
            std::string_view object_id,
            std::string_view object_hash,
            uintmax_t object_size,
            std::string_view object_mime_type
        );

        std::vector<object> delete_orphan_objects() const;

        bucket fetch_bucket(std::string_view bucket_name) const;

        std::vector<bucket> fetch_buckets(
            const std::vector<std::string>& names
        ) const;

        std::vector<bucket> fetch_buckets() const;

        store_totals get_store_totals() const;
    };
}
