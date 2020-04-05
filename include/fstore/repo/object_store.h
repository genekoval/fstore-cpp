#pragma once

#include <fstore/repo/bucket.h>
#include <fstore/repo/object.h>
#include <fstore/repo/store_totals.h>

namespace fstore::repo::db {
    struct object_store : public entix::connected<postgresql> {
        object_store(const std::string& options);

        void create_bucket(const std::string& bucket_name);

        std::vector<object> delete_orphan_objects();

        bucket fetch_bucket(const std::string& bucket_name);

        std::vector<bucket> fetch_buckets(
            const std::vector<std::string>& names
        );

        std::vector<bucket> fetch_buckets();

        store_totals get_store_totals();

        void truncate_buckets();

        void truncate_objects();
    };
}
