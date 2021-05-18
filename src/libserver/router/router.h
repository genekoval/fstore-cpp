#pragma once

#include "../context/context.h"

namespace fstore::server {
    using router_type = zipline::router<
        net::socket,
        net::event_t,
        net::error_list,
        context,
        decltype(&context::add_object),
        decltype(&context::create_object_from_file),
        decltype(&context::fetch_bucket),
        decltype(&context::get_object),
        decltype(&context::get_object_metadata),
        decltype(&context::remove_object)
    >;

    auto make_router(context& ctx) -> router_type;
}
