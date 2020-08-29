#include "endpoints/endpoints.h"

#include <fstore/server/server.h>

#include <netcore/server.h>
#include <timber/timber>

namespace fstore::server {
    static inline auto router() {
        return zipline::make_router<protocol, event_t>(
            endpoint::add_object,
            endpoint::create_object_from_file,
            endpoint::fetch_bucket,
            endpoint::get_object,
            endpoint::get_object_metadata,
            endpoint::remove_object
        );
    }

    connection::connection(
        const netcore::socket& sock,
        service::object_store& store
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
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void {
        const auto routes = router();

        auto server = netcore::server(
            [&routes, &store](netcore::socket&& sock) {
                routes.route(protocol(sock, store));
            }
        );

        server.listen(endpoint, callback);

        INFO() << "Shutting down...";
    }
}
