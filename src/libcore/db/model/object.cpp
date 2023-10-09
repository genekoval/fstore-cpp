#include <internal/core/db/model/object.hpp>

namespace fstore::core::db {
    object::operator fstore::object() {
        return fstore::object {
            .id = std::move(id),
            .hash = std::move(hash),
            .size = static_cast<uintmax_t>(size),
            .type = std::move(type),
            .subtype = std::move(subtype),
            .date_added = date_added};
    }
}

PGCPP_COMPOSITE_DEFINE(
    fstore::core::db::object,
    &fstore::core::db::object::id,
    &fstore::core::db::object::hash,
    &fstore::core::db::object::size,
    &fstore::core::db::object::type,
    &fstore::core::db::object::subtype,
    &fstore::core::db::object::date_added
);
