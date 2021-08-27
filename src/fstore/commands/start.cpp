#include "commands.h"

#include <fstore/cli.h>
#include <fstore/core/object_store.h>
#include <fstore/server/server.h>

#include <dmon/dmon>
#include <timber/timber>

static auto $start(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view conf,
    bool daemon,
    timber::level log_level
) -> void {
    const auto settings = fstore::conf::settings::load_file(conf);
    timber::reporting_level = log_level;

    if (daemon && !dmon::daemonize({
        .group = settings.daemon.group,
        .identifier = app.name,
        .pidfile = settings.daemon.pidfile,
        .user = settings.daemon.user
    })) return;

    NOTICE() << app.name << " version " << app.version << " starting up";

    auto store = fstore::core::object_store(
        settings.database.connection.str(),
        settings.objects_dir
    );

    fstore::server::listen(store, settings.server, []() {
        INFO() << "Server started. Listening for connections...";
    });

    NOTICE() << app.name << " shutting down";
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
                flag(
                    {"daemon", "d"},
                    "Run the program as a daemon"
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
