#include <internal/core/db/model/remove_result.hpp>

namespace fstore::core::db {
    remove_result::operator fstore::remove_result() const {
        return fstore::remove_result {
            .objects_removed = static_cast<size_t>(objects_removed),
            .space_freed = static_cast<uintmax_t>(space_freed)};
    }
}

PGCPP_COMPOSITE_DEFINE(
    fstore::core::db::remove_result,
    &fstore::core::db::remove_result::objects_removed,
    &fstore::core::db::remove_result::space_freed
);
