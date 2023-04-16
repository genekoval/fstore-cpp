#include <internal/server/server.hpp>

using netcore::address_type;
using netcore::endpoint;

namespace {
    auto on_error(
        const endpoint& endpoint,
        std::exception_ptr exception
    ) -> void {
        try {
            if (exception) std::rethrow_exception(exception);
        }
        catch (const std::exception& ex) {
            TIMBER_ERROR(ex.what());
        }
    }
}
namespace fstore::server {
    server_context::server_context(router_type& router) : router(&router) {}

    auto server_context::close() -> void {}

    auto server_context::connection(netcore::socket&& client) -> ext::task<> {
        auto socket = net::socket(std::forward<netcore::socket>(client));
        co_await router->route(socket);
    }

    auto server_context::listen(const address_type& address) -> void {
        TIMBER_INFO("Listening for connections on {}", address);
    }


    auto listen(
        router_type& router,
        std::span<const netcore::endpoint> endpoints
    ) -> ext::task<server_list> {
        auto servers = co_await server_list::listen(
            endpoints,
            [&router]() { return server(router); },
            on_error
        );

        if (servers.listening() == 0) throw std::runtime_error(
            "Failed to listen for connections"
        );

        co_return servers;
    }
}
