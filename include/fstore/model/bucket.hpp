#pragma once

#include "time_point.hpp"

#include <uuid++/zipline>

namespace fstore {
    struct bucket {
        UUID::uuid id;
        std::string name;
        time_point date_created;
        std::size_t size;
        uintmax_t space_used;

        auto operator<=>(const bucket&) const = default;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::bucket,
        &fstore::bucket::id,
        &fstore::bucket::name,
        &fstore::bucket::date_created,
        &fstore::bucket::size,
        &fstore::bucket::space_used
    );
}
