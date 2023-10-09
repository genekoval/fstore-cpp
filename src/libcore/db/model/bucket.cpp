#include <internal/core/db/model/bucket.hpp>

namespace fstore::core::db {
    bucket::operator fstore::bucket() {
        return fstore::bucket {
            .id = std::move(id),
            .name = std::move(name),
            .date_created = date_created,
            .size = static_cast<size_t>(size),
            .space_used = static_cast<uintmax_t>(space_used)};
    }
}

PGCPP_COMPOSITE_DEFINE(
    fstore::core::db::bucket,
    &fstore::core::db::bucket::id,
    &fstore::core::db::bucket::name,
    &fstore::core::db::bucket::date_created,
    &fstore::core::db::bucket::size,
    &fstore::core::db::bucket::space_used
);
