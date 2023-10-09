#include <fstore/object_store.hpp>

namespace fstore {
    part::part(client_type& out) : out(out) {}

    auto part::await_write() -> ext::task<> { return out.await_write(); }

    auto part::try_write(std::span<const std::byte> data) -> std::size_t {
        return out.try_write(data.data(), data.size());
    }

    auto part::write(std::span<const std::byte> data) -> ext::task<> {
        co_await out.write(data.data(), data.size());
    }
}
