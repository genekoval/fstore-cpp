#pragma once

#include <fstore/net/zipline/protocol.h>
#include <fstore/service/object_store.h>

#include <string_view>
#include <sys/sendfile.h>

namespace fstore::server {
    struct protocol : zipline::protocol<net::socket> {
        service::object_store* store;

        protocol(
            net::socket& sock,
            service::object_store& store
        ) :
            zipline::protocol<net::socket>(sock),
            store(&store)
        {}

        auto sendfile(const netcore::fd& fd, std::size_t count) -> void {
            write(count);
            sock->flush();

            const auto bytes = ::sendfile(sock->socket(), fd, NULL, count);

            DEBUG() << *sock << " send " << bytes << " bytes (sendfile)";
        }
    };

    auto listen(
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void;
}
