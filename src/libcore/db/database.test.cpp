#include <internal/core/db/database.test.hpp>

namespace fstore::test::db {
    database::database(const std::shared_ptr<mock_connection>& connection) :
        connection(connection) {}

    auto database::connect() -> ext::task<core::db::connection_wrapper> {
        co_return core::db::connection_wrapper(connection);
    }
}
