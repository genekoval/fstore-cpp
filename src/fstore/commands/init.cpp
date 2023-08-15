#include "commands.hpp"
#include "../db/db.hpp"
#include "../options/opts.hpp"

using namespace commline;

namespace {
    namespace internal {
        auto init(
            const app& app,
            std::string_view confpath,
            bool overwrite
        ) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);

            netcore::run([&app, &settings, overwrite]() -> ext::task<> {
                auto db = fstore::cli::database(settings);

                if (overwrite) co_await db.reset(app.version);
                else co_await db.init(app.version);
            }());
        }
    }
}

namespace fstore::cli {
    auto init(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "init",
            "Initialize the database",
            options(
                opts::config(confpath),
                flag({"o", "overwrite"}, "Delete existing data if necessary")
            ),
            arguments(),
            internal::init
        );
    }
}
