#pragma once

#include <netcore/netcore>
#include <zipline/zipline>

namespace fstore::net {
    template <typename T>
    concept writer = zipline::io::writer<T> && requires(
        T& t,
        const netcore::fd& fd,
        std::size_t count
    ) {
        { t.sendfile(fd, count) } -> std::same_as<ext::task<>>;
    };

    using socket = netcore::buffered_socket;

    static_assert(zipline::io::reader<socket>);
    static_assert(writer<socket>);
}
