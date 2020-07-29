#include "endpoints/endpoints.h"

#include <fstore/server/server.h>

#include <netcore/server.h>
#include <timber/timber>

namespace fstore::server {
    static inline auto router() {
        return zipline::make_router<protocol, event_t>(
            message,
            print,
            get_store_totals
        );
    }

    connection::connection(
        const netcore::socket& sock,
        core::object_store& store
    ) :
        sock(&sock),
        store(&store)
    {}

    auto connection::recv(void* buffer, std::size_t len) const -> void {
        sock->recv(buffer, len);
    }

    auto connection::send(const void* buffer, std::size_t len) const -> void {
        sock->send(buffer, len);
    }

    auto listen(
        core::object_store& store,
        std::string_view endpoint
    ) -> void {
        const auto routes = router();

        auto server = netcore::server(
            [&routes, &store](netcore::socket&& sock) {
                routes.route(connection(sock, store));
            }
        );

        server.listen(endpoint);

        INFO() << "Shutting down...";
    }
}
