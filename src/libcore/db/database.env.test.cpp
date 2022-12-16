#include <internal/conf/settings.env.test.hpp>
#include <internal/core/db/database.env.test.hpp>

namespace {
    constexpr auto connection_count = 1;

    auto connection_string() -> const std::string& {
        static auto instance = fstore::test::SettingsEnvironment::settings()
            .database
            .connection
            .str();

        return instance;
    }
}

namespace fstore::test {
    auto DatabaseEnvironment::connection() -> pqxx::connection& {
        static auto instance = pqxx::connection(connection_string());
        return instance;
    }

    auto DatabaseEnvironment::database() -> core::db::database& {
        static auto instance = core::db::database(
            connection_string(),
            connection_count
        );
        return instance;
    }

    auto DatabaseEnvironment::SetUp() -> void {
        connection();
        database();
    }
}
