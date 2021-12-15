#include "context/context.h"

#include <fstore/net/zipline/transfer.h>
#include <fstore/server/server.h>

#include <timber/timber>

using fstore::server::context;

namespace {
    template <typename ...Routes>
    using router = zipline::router<
        fstore::net::socket,
        fstore::net::event_t,
        fstore::net::error_list,
        context,
        Routes...
    >;

    auto make_router(context& ctx) {
        return router(
            ctx,
            &context::add_object,
            &context::create_object_from_file,
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
    auto listen(
        core::object_store& store,
        const server_info& info,
        const netcore::unix_socket& unix_socket,
        const std::function<void()>& callback
    ) -> void {
        auto ctx = context(store, info);
        auto routes = make_router(ctx);

        auto server = netcore::server([&routes](netcore::socket&& sock) {
            auto socket = net::socket(std::move(sock));
            routes.route(socket);
        });

        server.listen(unix_socket, callback);
    }
}
