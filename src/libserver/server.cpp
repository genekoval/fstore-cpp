#include "context/context.h"

#include <fstore/net/zipline/coder.h>
#include <fstore/server/server.h>

#include <timber/timber>

using fstore::server::context;

namespace {
    template <typename ...Routes>
    auto make_router(context&& ctx, Routes&&... routes) {
        return zipline::router<
            fstore::net::socket,
            fstore::net::event_t,
            fstore::net::error_list,
            context,
            Routes...
        >(
            std::move(ctx),
            routes...
        );
    }


    auto make_router(
        fstore::core::object_store& store,
        const fstore::server::server_info& info
    ) {
        return make_router(
            context(store, info),
            &context::add_object,
            &context::fetch_bucket,
            &context::get_object,
            &context::get_object_metadata,
            &context::get_server_info,
            &context::remove_object,
            &context::remove_objects
        );
    }
}

namespace fstore::server {
    auto create(
        core::object_store& store,
        const server_info& info
    ) -> netcore::server {
        return netcore::server([
            routes = make_router(store, info)
        ](netcore::socket&& socket) mutable -> ext::task<> {
            auto client = net::socket(std::forward<netcore::socket>(socket));
            co_await routes.route(client);
        });
    }
}
