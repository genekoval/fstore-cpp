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
}
