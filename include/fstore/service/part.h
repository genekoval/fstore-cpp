#pragma once

#include <fstream>
#include <span>

namespace fstore::service {
    class part {
        std::ofstream out;
    public:
        const std::string id;

        part(std::string_view id, std::ofstream&& out);

        auto write(std::span<const std::byte> data) -> void;
    };
}
