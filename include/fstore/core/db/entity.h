#pragma once

#include "model.h"

#include <entix/entix>

namespace entix {
    ENTITY(fstore::core::db::bucket);
    ENTITY(fstore::core::db::object);
    ENTITY(fstore::core::db::object_error);
    ENTITY(fstore::core::db::remove_result);
    ENTITY(fstore::core::db::store_totals);
}
