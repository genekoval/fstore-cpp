#include "commands.h"
#include "../db/db.h"
#include "../options/opts.h"

using namespace commline;

namespace {
    auto $db(
        const app& app,
        std::string_view confpath,
        const std::vector<std::string_view>& commands
    ) -> void {
        const auto settings = fstore::conf::settings::load_file(confpath);
        const auto db = fstore::cli::database(settings);

        db.exec(commands);
    }
}

namespace fstore::cli {
    auto db(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "db",
            "Connect to the database using the psql client",
            options(
                opts::config(confpath)
            ),
            arguments(
                variadic<std::string_view>("command")
            ),
            $db
        );
    }
}
