#pragma once

#include <cstdint> // uintmax_t

namespace fstore::core {
    struct store_totals {
        virtual int bucket_count() const = 0;
        virtual int object_count() const = 0;
        virtual uintmax_t space_used() const = 0;
    };
}
