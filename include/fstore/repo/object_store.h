#pragma once

#include <fstore/repo/bucket.h>
#include <fstore/repo/object.h>
#include <fstore/repo/store_totals.h>

#include <string_view>

namespace fstore::repo::db {
    struct object_store : public entix::connected<postgresql> {
        object_store(std::string_view options);

        void create_bucket(std::string_view bucket_name);

        std::vector<object> delete_orphan_objects();

        bucket fetch_bucket(std::string_view bucket_name);

        std::vector<bucket> fetch_buckets(
            const std::vector<std::string>& names
        );

        std::vector<bucket> fetch_buckets();

        store_totals get_store_totals();

        void truncate_buckets();

        void truncate_objects();
    };
}
