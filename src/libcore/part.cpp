#include <fstore/core/part.h>

namespace fstore::core {
    part::part(const UUID::uuid& id, std::ofstream&& out) :
        out(std::move(out)),
        id(id)
    {}

    auto part::write(std::span<const std::byte> data) -> void {
        out.write(
            reinterpret_cast<const char*>(data.data()),
            data.size()
        );
    }
}
