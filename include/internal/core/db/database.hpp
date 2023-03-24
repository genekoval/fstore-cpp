#pragma once

#include "connection.hpp"

#ifdef TEST
#include "connection_wrapper.hpp"
#endif

namespace fstore::core::db {
    class database {
        class provider {
            pg::parameters params;
        public:
            provider() = default;

            provider(const pg::parameters& params);

            auto provide() -> ext::task<pg::client>;
        };

        ext::async_pool<pg::client, provider> pool;
    public:
        database() = default;

        database(const pg::parameters& params);

        VIRTUAL_DESTRUCTOR(database)

        VIRTUAL auto connect() -> ext::task<
#ifdef TEST
            connection_wrapper
#else
            connection
#endif
        >;
    };
}
