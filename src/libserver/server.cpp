#include <internal/server/server.hpp>

using netcore::address_type;

namespace fstore::server {
    server_context::server_context(router_type&& router) :
        router(std::forward<router_type>(router))
    {}

    auto server_context::close() -> void {}

    auto server_context::connection(netcore::socket&& client) -> ext::task<> {
        auto socket = net::socket(std::forward<netcore::socket>(client));
        co_await router.route(socket);
    }

    auto server_context::listen(const address_type& address) -> void {
        TIMBER_INFO("Listening for connections on {}", address);
    }

    auto create(
        core::object_store& store,
        const server_info& info
    ) -> server_type {
        return server_type(make_router(store, info));
    }
}
