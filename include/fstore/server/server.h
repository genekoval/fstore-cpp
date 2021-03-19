#pragma once

#include <fstore/service/object_store.h>

#include <netcore/fd.h>
#include <netcore/socket.h>
#include <string_view>
#include <sys/sendfile.h>
#include <zipline/zipline>

namespace fstore::server {
    using event_t = unsigned int;

    struct protocol : zipline::protocol<netcore::socket> {
        service::object_store* store;

        protocol(
            netcore::socket& sock,
            service::object_store& store
        ) :
            zipline::protocol<netcore::socket>(sock),
            store(&store)
        {}

        auto sendfile(const netcore::fd& fd, std::size_t count) -> void {
            write(count);
            const auto bytes = ::sendfile(*sock, fd, NULL, count);

            DEBUG() << *sock << " send " << bytes << " bytes (sendfile)";
        }
    };

    auto listen(
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void;
}
