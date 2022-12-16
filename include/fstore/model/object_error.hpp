#pragma once

#include <uuid++/zipline>

namespace fstore {
    struct object_error {
        UUID::uuid id;
        std::string message;

        auto operator<=>(const object_error&) const = default;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::object_error,
        &fstore::object_error::id,
        &fstore::object_error::message
    );
}
