#include <fstore/server/server.h>

namespace fstore::server {
    protocol::protocol(
        net::socket& sock,
        service::object_store& store
    ) :
        zipline::protocol<net::socket>(sock),
        store(&store)
    {}

    auto protocol::sendfile(const netcore::fd& fd, std::size_t count) -> void {
        write(count);
        sock->flush();
        sock->socket().sendfile(fd, count);
    }
}
