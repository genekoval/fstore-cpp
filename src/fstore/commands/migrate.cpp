#include "commands.h"
#include "../db/db.h"
#include "../options/opts.h"

using namespace commline;

namespace {
    auto $migrate(
        const app& app,
        std::string_view confpath
    ) -> void {
        const auto settings = fstore::conf::settings::load_file(confpath);
        const auto db = fstore::cli::database(settings);

        db.migrate(app.version);
    }
}

namespace fstore::cli {
    auto migrate(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Update schemas to the current program version",
            options(
                opts::config(confpath)
            ),
            arguments(),
            $migrate
        );
    }
}
