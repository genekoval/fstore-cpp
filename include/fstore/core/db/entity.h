#pragma once

#include "model.h"

#include <entix/entix>

namespace entix {
    ENTITY(
        fstore::core::db::bucket,
        &fstore::core::db::bucket::id,
        &fstore::core::db::bucket::name,
        &fstore::core::db::bucket::date_created,
        &fstore::core::db::bucket::size,
        &fstore::core::db::bucket::space_used
    );

    ENTITY(
        fstore::core::db::object,
        &fstore::core::db::object::id,
        &fstore::core::db::object::hash,
        &fstore::core::db::object::size,
        &fstore::core::db::object::type,
        &fstore::core::db::object::subtype,
        &fstore::core::db::object::date_added
    );

    ENTITY(
        fstore::core::db::object_error,
        &fstore::core::db::object_error::id,
        &fstore::core::db::object_error::message
    );

    ENTITY(
        fstore::core::db::remove_result,
        &fstore::core::db::remove_result::objects_removed,
        &fstore::core::db::remove_result::space_freed
    );

    ENTITY(
        fstore::core::db::store_totals,
        &fstore::core::db::store_totals::buckets,
        &fstore::core::db::store_totals::objects,
        &fstore::core::db::store_totals::space_used
    );
}
