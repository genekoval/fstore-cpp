#include "../api/api.hpp"
#include "../db/db.hpp"
#include "../options/opts.hpp"
#include "commands.hpp"

#include <internal/cli.hpp>

#include <fmt/format.h>

using namespace commline;

namespace {
    namespace internal {
        auto archive(
            const app& app,
            std::string_view confpath,
            std::optional<std::string_view> output,
            timber::level log_level
        ) -> void {
            timber::reporting_level = log_level;

            const auto settings = fstore::conf::settings::load_file(confpath);

            const auto archive = output.value_or(settings.archive.path);
            if (archive.empty())
                throw std::runtime_error("archive location not specified");

            const auto dump_file = fstore::cli::dump_file(settings);

            netcore::run([&dump_file, &settings]() -> ext::task<> {
                auto db = fstore::cli::database(settings);
                co_await db.dump(dump_file);
            }());

            const auto fs = fstore::core::fs::filesystem(settings.home);

            auto options = std::vector<std::string_view>();
            for (const auto& option : settings.archive.options) {
                options.emplace_back(option);
            }

            options.push_back(dump_file);

            fs.sync(settings.archive.sync, options, archive);

            std::filesystem::remove(dump_file);
        }
    }
}

namespace fstore::cli {
    auto archive(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Create a backup of the database and object files",
            options(
                opts::config(confpath),
                option<std::optional<std::string_view>>(
                    {"o", "output"},
                    "Send data to the specified directory",
                    "directory"
                ),
                opts::log_level()
            ),
            arguments(),
            internal::archive
        );
    }
}
