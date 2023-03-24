#pragma once

#include <fstore/model/object.hpp>

#include <pg++/pg++>

namespace fstore::core::db {
    struct object {
        UUID::uuid id;
        std::string hash;
        std::int64_t size;
        std::string type;
        std::string subtype;
        time_point date_added;

        auto operator<=>(const object&) const = default;

        operator fstore::object();
    };
}

PGCPP_COMPOSITE_DECL(fstore::core::db::object, "object");
