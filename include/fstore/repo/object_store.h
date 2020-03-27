#pragma once

#include <fstore/repo/bucket.h>
#include <fstore/repo/object.h>
#include <fstore/repo/store_totals.h>

#include <string_view>

namespace fstore::repo::db {
    struct object_store {
        void create_bucket(std::string_view bucket_name) const;

        std::vector<object> delete_orphan_objects() const;

        bucket fetch_bucket(std::string_view bucket_name) const;

        std::vector<bucket> fetch_buckets(
            const std::vector<std::string>& names
        ) const;

        std::vector<bucket> fetch_buckets() const;

        store_totals get_store_totals() const;

        void truncate_buckets() const;

        void truncate_objects() const;
    };
}
