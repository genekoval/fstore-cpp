#include <fstore/core/db/entity.h>

using namespace fstore::core::db;

namespace entix {
    READ_ENTITY(
        bucket,
        &bucket::id,
        &bucket::name,
        &bucket::date_created,
        &bucket::size,
        &bucket::space_used
    );

    READ_ENTITY(
        object,
        &object::id,
        &object::hash,
        &object::size,
        &object::type,
        &object::subtype,
        &object::date_added
    );

    READ_ENTITY(
        remove_result,
        &remove_result::objects_removed,
        &remove_result::space_freed
    );

    READ_ENTITY(
        store_totals,
        &store_totals::buckets,
        &store_totals::objects,
        &store_totals::space_used
    );
}
