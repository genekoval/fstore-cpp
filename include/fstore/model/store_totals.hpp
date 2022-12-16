#pragma once

#include <zipline/zipline>

namespace fstore {
    struct store_totals {
        std::size_t buckets;
        std::size_t objects;
        uintmax_t space_used;

        auto operator<=>(const store_totals&) const = default;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::store_totals,
        &fstore::store_totals::buckets,
        &fstore::store_totals::objects,
        &fstore::store_totals::space_used
    );
}
