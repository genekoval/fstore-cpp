#pragma once

#include <fstore/service/object_store.h>

#include <netcore/socket.h>
#include <string_view>
#include <zipline/zipline>

namespace fstore::server {
    using event_t = unsigned int;

    class connection {
        const netcore::socket* sock;
    public:
        service::object_store* store;

        connection(
            const netcore::socket& sock,
            service::object_store& store
        );

        auto recv(void* buffer, std::size_t len) const -> void;

        auto send(const void* buffer, std::size_t len) const -> void;
    };

    struct protocol : zipline::protocol<netcore::socket> {
        service::object_store* store;

        protocol(
            const netcore::socket& sock,
            service::object_store& store
        ) :
            zipline::protocol<netcore::socket>(sock),
            store(&store)
        {}
    };

    auto listen(
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void;
}
