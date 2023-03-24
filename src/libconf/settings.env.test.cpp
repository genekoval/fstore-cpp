#include <internal/conf/settings.env.test.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace {
    constexpr auto settings_file = ".test.conf.yaml";
    const auto settings_path = fs::current_path() / settings_file;
}

namespace fstore::test {
    auto SettingsEnvironment::db_params() -> const pg::parameters& {
        static const auto instance = pg::parameters::parse(
            settings().database.connection.parameters
        );

        return instance;
    }

    auto SettingsEnvironment::settings() -> const conf::settings& {
        static const auto instance = conf::settings::load_file(
            settings_path.native()
        );

        return instance;
    }

    auto SettingsEnvironment::SetUp() -> void {
        settings();
    }
}
