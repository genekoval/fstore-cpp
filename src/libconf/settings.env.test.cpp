#include <fstore/conf/settings.env.test.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace {
    constexpr auto settings_file = ".test.conf.yaml";
    const auto settings_path = fs::current_path() / settings_file;
}

namespace fstore::test {
    auto SettingsEnvironment::settings() -> const conf::settings& {
        static auto instance = conf::settings::load_file(
            settings_path.native()
        );

        return instance;
    }

    auto SettingsEnvironment::SetUp() -> void {
        settings();
    }
}
