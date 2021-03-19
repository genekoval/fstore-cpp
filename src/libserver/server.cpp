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
