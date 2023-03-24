#pragma once

#include <fstore/model/remove_result.hpp>

#include <pg++/pg++>

namespace fstore::core::db {
    struct remove_result {
        std::int64_t objects_removed;
        std::int64_t space_freed;

        auto operator<=>(const remove_result&) const = default;

        operator fstore::remove_result() const;
    };
}

PGCPP_COMPOSITE_DECL(fstore::core::db::remove_result, "remove_result");
