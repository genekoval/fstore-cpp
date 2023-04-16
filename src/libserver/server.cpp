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
        timber::timer& startup_timer,
        std::span<const netcore::endpoint> endpoints,
        core::object_store& store,
        const server_info& info
    ) -> ext::task<> {
        auto router = make_router(store, info);

        auto servers = co_await server_list::listen(
            endpoints,
            [&router]() { return server(router); },
            on_error
        );

        if (servers.listening() == 0) throw std::runtime_error(
            "Failed to listen for connections"
        );

        startup_timer.stop();
        auto uptime_timer = timber::timer(
            "Server shutting down. Up",
            timber::level::notice
        );

        co_await servers.join();

        uptime_timer.stop();
    }
}
