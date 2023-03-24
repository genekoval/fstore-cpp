#pragma once

#include <fstore/model/store_totals.hpp>

#include <pg++/pg++>

namespace fstore::core::db {
    struct store_totals {
        std::int64_t buckets;
        std::int64_t objects;
        std::int64_t space_used;

        auto operator<=>(const store_totals&) const = default;

        operator fstore::store_totals() const;
    };
}

PGCPP_COMPOSITE_DECL(fstore::core::db::store_totals, "store_totals");
