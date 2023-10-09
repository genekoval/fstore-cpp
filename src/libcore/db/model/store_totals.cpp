#include <internal/core/db/model/store_totals.hpp>

namespace fstore::core::db {
    store_totals::operator fstore::store_totals() const {
        return fstore::store_totals {
            .buckets = static_cast<size_t>(buckets),
            .objects = static_cast<size_t>(objects),
            .space_used = static_cast<uintmax_t>(space_used)};
    }
}

PGCPP_COMPOSITE_DEFINE(
    fstore::core::db::store_totals,
    &fstore::core::db::store_totals::buckets,
    &fstore::core::db::store_totals::objects,
    &fstore::core::db::store_totals::space_used
);
