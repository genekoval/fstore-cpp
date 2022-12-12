#include <fstore/client.h>

namespace fstore {
    part::part(client_type& out) : out(out) {}

    auto part::write(std::span<const std::byte> data) -> ext::task<> {
        co_await out.write_all(data);
    }
}
