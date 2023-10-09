#include <internal/core/fs/part.hpp>

#include <timber/timber>

namespace fstore::core::fs {
    part::part(const UUID::uuid& id, std::ofstream&& out) :
        out(std::move(out)),
        id(id) {}

    auto part::write(std::span<const std::byte> data) -> void {
        TIMBER_TRACE("part ({}) write {:L} bytes", id, data.size());

        out.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
}
