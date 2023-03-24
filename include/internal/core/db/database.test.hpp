#pragma once

#include "mock_connection.test.hpp"
#include "database.hpp"

namespace fstore::test::db {
    class database final : public core::db::database {
        std::shared_ptr<mock_connection> connection;
    public:
        database(const std::shared_ptr<mock_connection>& connection);

        auto connect() -> ext::task<core::db::connection_wrapper> override;
    };
}
