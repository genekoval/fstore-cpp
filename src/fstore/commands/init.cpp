#include "commands.hpp"
#include "../db/db.hpp"
#include "../options/opts.hpp"

using namespace commline;

namespace {
    auto $init(
        const app& app,
        std::string_view confpath
    ) -> void {
        const auto settings = fstore::conf::settings::load_file(confpath);
        const auto db = fstore::cli::database(settings);

        db.init(app.version);
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
                opts::config(confpath)
            ),
            arguments(),
            $init
        );
    }
}
