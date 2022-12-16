#pragma once

#include <cstdint>

namespace fstore {
    enum class event : std::uint32_t {
        add_object,
        fetch_bucket,
        get_object,
        get_object_metadata,
        get_server_info,
        remove_object,
        remove_objects
    };
}
