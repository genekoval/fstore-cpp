#include <internal/core/db/database.hpp>

#include <ext/string.h>

namespace fstore::core::db {
    database::database(const pg::parameters& params) :
        pool(ext::pool_options(), params) {}

#ifdef TEST
    auto database::connect() -> ext::task<connection_wrapper> {
        co_return connection_wrapper(std::shared_ptr<connection>(
            new connection(co_await pool.checkout())
        ));
    }
#else
    auto database::connect() -> ext::task<connection> {
        co_return connection(co_await pool.checkout());
    }
#endif
}
