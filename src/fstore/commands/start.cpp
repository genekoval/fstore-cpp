#include "commands.h"

#include <fstore/cli.h>
#include <fstore/server/server.h>
#include <fstore/service/object_store.h>

#include <timber/timber>
#include <unistd.h>

static auto $start(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view conf,
    timber::level log_level
) -> void {
    const auto settings = fstore::conf::settings::load_file(conf);
    timber::reporting_level = log_level;

    INFO()
        << app.name
        << " version " << app.version
        << " starting: [PID " << getpid() << ']';

    auto store = fstore::service::object_store(
        settings.database,
        settings.objects_dir
    );

    fstore::server::listen(store, settings.server, []() {
        INFO() << "Server started. Listening for connections...";
    });
}

namespace fstore::cli {
    using namespace commline;

    auto start(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "start",
            "Start the server.",
            options(
                option<std::string_view>(
                    {"conf", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                ),
                option<timber::level>(
                    {"log-level", "l"},
                    "Minimum log level to display.",
                    "log level",
                    timber::level::info
                )
            ),
            $start
        );
    }
}
