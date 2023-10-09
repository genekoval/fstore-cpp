#include "../db/db.hpp"
#include "../options/opts.hpp"
#include "commands.hpp"

#include <internal/cli.hpp>

using namespace commline;

namespace {
    namespace internal {
        auto migrate(
            const app& app,
            std::string_view confpath,
            timber::level log_level
        ) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            timber::reporting_level = log_level;

            netcore::run([&app, &settings]() -> ext::task<> {
                auto db = fstore::cli::database(settings);
                co_await db.migrate(app.version);
            }());
        }
    }
}

namespace fstore::cli {
    auto migrate(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Update schemas to the current program version",
            options(opts::config(confpath), opts::log_level()),
            arguments(),
            internal::migrate
        );
    }
}
