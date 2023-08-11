#pragma once

#include "time_point.hpp"

#include <ext/json.hpp>
#include <uuid++/json.hpp>
#include <uuid++/zipline>

namespace fstore {
    struct object {
        UUID::uuid id;
        std::string hash;
        uintmax_t size;
        std::string type;
        std::string subtype;
        time_point date_added;

        auto operator<=>(const object&) const = default;

        auto mime_type() const -> std::string;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        object,
        id,
        hash,
        size,
        type,
        subtype,
        date_added
    );
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::object,
        &fstore::object::id,
        &fstore::object::hash,
        &fstore::object::size,
        &fstore::object::type,
        &fstore::object::subtype,
        &fstore::object::date_added
    );
}
