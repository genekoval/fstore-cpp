#pragma once

#include <fstore/model/bucket.hpp>

#include <pg++/pg++>

namespace fstore::core::db {
    struct bucket {
        UUID::uuid id;
        std::string name;
        time_point date_created;
        std::int64_t size;
        std::int64_t space_used;

        auto operator<=>(const bucket&) const = default;

        operator fstore::bucket();
    };
}

PGCPP_COMPOSITE_DECL(fstore::core::db::bucket, "bucket");
