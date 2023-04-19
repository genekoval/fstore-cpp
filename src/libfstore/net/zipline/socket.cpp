#include <fstore/net/zipline/socket.hpp>

namespace fstore::net {
    auto socket::flush() -> ext::task<> {
        return inner.flush();
    }

    auto socket::read(
        std::size_t len
    ) -> ext::task<std::span<const std::byte>> {
        return inner.read(len);
    }

    auto socket::read(void* dest, std::size_t len) -> ext::task<> {
        return inner.read(dest, len);
    }

    auto socket::sendfile(
        const netcore::fd& fd,
        std::size_t count
    ) -> ext::task<> {
        return inner.inner.sendfile(fd, count);
    }

    auto socket::write(const void* src, std::size_t len) -> ext::task<> {
        return inner.write(src, len);
    }
}
