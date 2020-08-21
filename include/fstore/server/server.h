#pragma once

#include <fstore/service/object_store.h>

#include <netcore/fd.h>
#include <netcore/socket.h>
#include <string_view>
#include <sys/sendfile.h>
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

        auto sendfile(const netcore::fd& fd, std::size_t count) -> void {
            write(count);
            const auto bytes = ::sendfile(sock->fd(), fd, NULL, count);

            DEBUG() << *sock << " send " << bytes << " bytes (sendfile)";
        }
    };

    auto listen(
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void;
}
