#include "commands.h"

#include <fstore/conf/settings.h>

#include <dmon/dmon>

using namespace commline;

namespace {
    auto $stop(
        const app& app,
        const argv&,
        std::string_view confpath
    ) -> void {
        const auto settings = fstore::conf::settings::load_file(confpath);
        dmon::stop(settings.daemon.pidfile);
    }
}

namespace fstore::cli {
    auto stop(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "stop",
            "Stop the server daemon",
            options(
                option<std::string_view>(
                    {"config", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $stop
        );
    }
}
