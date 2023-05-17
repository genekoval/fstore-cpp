#pragma once

#include <ext/pool>
#include <pg++/pg++>

namespace fstore::core::db {
    class provider {
        pg::parameters params;
    public:
        provider() = default;

        provider(const pg::parameters& params);

        auto provide() -> ext::task<pg::client>;
    };

    using pool = ext::pool<provider>;
}
