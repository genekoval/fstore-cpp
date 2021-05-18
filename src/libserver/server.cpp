#include "router/router.h"

#include <fstore/server/server.h>
#include <fstore/server/transfer.h>

#include <timber/timber>

namespace fstore::server {
    auto listen(
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void {
        auto ctx = context(store);
        auto router = make_router(ctx);

        auto server = netcore::server([&router](netcore::socket&& sock) {
            auto socket = net::socket(std::move(sock));
            router.route(socket);
        });

        server.listen(endpoint, callback);

        INFO() << "Shutting down...";
    }
}
