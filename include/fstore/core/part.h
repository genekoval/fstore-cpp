#pragma once

#include <fstream>
#include <span>
#include <uuid++/uuid++>

namespace fstore::core {
    class part {
        std::ofstream out;
    public:
        const UUID::uuid id;

        part(const UUID::uuid& id, std::ofstream&& out);

        auto write(std::span<const std::byte> data) -> void;
    };
}
