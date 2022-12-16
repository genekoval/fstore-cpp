#pragma once

#include <zipline/zipline>

namespace fstore {
    struct remove_result {
        std::size_t objects_removed;
        uintmax_t space_freed;

        auto operator<=>(const remove_result&) const = default;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::remove_result,
        &fstore::remove_result::objects_removed,
        &fstore::remove_result::space_freed
    );
}
