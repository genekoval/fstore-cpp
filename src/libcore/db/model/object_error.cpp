#include <internal/core/db/model/object_error.hpp>

PGCPP_COMPOSITE_DEFINE(
    fstore::object_error,
    &fstore::object_error::id,
    &fstore::object_error::message
);
