#pragma once

#include <ext/data_size.h>
#include <netcore/netcore>
#include <zipline/zipline>

namespace fstore::net {
    using namespace ext::literals;

    template <typename T>
    concept writer = zipline::io::writer<T> && requires(
        T& t,
        const netcore::fd& fd,
        std::size_t count
    ) {
        { t.sendfile(fd, count) } -> std::same_as<ext::task<>>;
    };

    struct socket {
        zipline::io::buffered<netcore::socket, 8_KiB> inner;

        template <typename... Args>
        socket(Args&&... args) : inner(std::forward<Args>(args)...) {}

        auto deregister() -> void;

        auto flush() -> ext::task<>;

        auto read(std::size_t len) -> ext::task<std::span<const std::byte>>;

        auto read(void* dest, std::size_t len) -> ext::task<>;

        auto register_scoped() -> netcore::register_guard;

        auto sendfile(
            const netcore::fd& fd,
            std::size_t count
        ) -> ext::task<>;

        auto write(const void* src, std::size_t len) -> ext::task<>;
    };

    static_assert(zipline::io::reader<socket>);
    static_assert(writer<socket>);
}
