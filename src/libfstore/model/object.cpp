#include <fstore/model/object.hpp>

#include <fmt/format.h>

namespace fstore {
    auto object::mime_type() const -> std::string {
        return fmt::format("{}/{}", type, subtype);
    }
}
