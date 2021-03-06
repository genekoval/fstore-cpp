#include <fstore/client.h>

namespace fstore {
    part::part(client& out) : out(&out) {}

    auto part::write(std::span<const std::byte> data) -> void {
        out->write_bytes(data);
    }
}
