#include "commands.h"
#include "../db/db.h"
#include "../options/opts.h"

using namespace commline;

namespace {
    auto $init(
        const app& app,
        std::string_view confpath
    ) -> void {
        const auto settings = fstore::conf::settings::load_file(confpath);
        const auto client = fstore::cli::data::client(settings);

        client.init();
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
