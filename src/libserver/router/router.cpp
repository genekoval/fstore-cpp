#include "router.h"

#include <fstore/net/zipline/transfer.h>

namespace fstore::server {
    auto make_router(context& ctx) -> router_type {
        return router_type(
            ctx,
            &context::add_object,
            &context::create_object_from_file,
            &context::fetch_bucket,
            &context::get_object,
            &context::get_object_metadata,
            &context::remove_object
        );
    }
}
