#include <internal/conf/settings.env.test.hpp>
#include <internal/core/fs/filesystem.env.test.hpp>

#include <filesystem>
#include <fmt/chrono.h>
#include <fmt/os.h>
#include <gtest/gtest.h>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    const auto log_path = fs::temp_directory_path() / "fstore.test.log";
    auto log_file = fmt::output_file(log_path.native());

    auto file_logger(const timber::log& log) noexcept -> void {
        log_file.print("{:%b %d %r}", log.timestamp);
        log_file.print(" {:>9}  ", log.log_level);
        log_file.print("{}\n", log.message);

        log_file.flush();
    }
}

auto main(int argc, char** argv) -> int {
    timber::log_handler = &file_logger;

    const auto runtime = netcore::runtime();

    testing::InitGoogleTest(&argc, argv);
    testing::AddGlobalTestEnvironment(new fstore::test::SettingsEnvironment);
    testing::AddGlobalTestEnvironment(new fstore::test::FilesystemEnvironment);

    return RUN_ALL_TESTS();
}
