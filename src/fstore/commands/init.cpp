#include "commands.h"
#include "../db/db.h"

using namespace commline;

namespace {
    auto $init(
        const app& app,
        const argv& argv,
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
                option<std::string_view>(
                    {"config", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $init
        );
    }
}
