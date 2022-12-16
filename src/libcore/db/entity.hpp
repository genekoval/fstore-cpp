#pragma once

#include <fstore/model/bucket.hpp>
#include <fstore/model/object.hpp>
#include <fstore/model/object_error.hpp>
#include <fstore/model/remove_result.hpp>
#include <fstore/model/store_totals.hpp>

#include <entix/entix>

namespace entix {
    ENTITY(
        fstore::bucket,
        &fstore::bucket::id,
        &fstore::bucket::name,
        &fstore::bucket::date_created,
        &fstore::bucket::size,
        &fstore::bucket::space_used
    );

    ENTITY(
        fstore::object,
        &fstore::object::id,
        &fstore::object::hash,
        &fstore::object::size,
        &fstore::object::type,
        &fstore::object::subtype,
        &fstore::object::date_added
    );

    ENTITY(
        fstore::object_error,
        &fstore::object_error::id,
        &fstore::object_error::message
    );

    ENTITY(
        fstore::remove_result,
        &fstore::remove_result::objects_removed,
        &fstore::remove_result::space_freed
    );

    ENTITY(
        fstore::store_totals,
        &fstore::store_totals::buckets,
        &fstore::store_totals::objects,
        &fstore::store_totals::space_used
    );
}
