#include <fstore/repo/entity.h>

using namespace fstore::repo;

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
        &object::mime_type,
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
