#include "commands.h"
#include "../api/api.h"
#include "../options/opts.h"

#include <fstore/cli.h>
#include <fstore/core/object_store.h>
#include <fstore/server/server.h>

#include <dmon/dmon>
#include <timber/timber>

namespace {
    auto $start(
        const commline::app& app,
        std::string_view conf,
        bool daemon,
        timber::level log_level
    ) -> void {
        const auto settings = fstore::conf::settings::load_file(conf);
        auto api = fstore::cli::api_container(settings);
        auto& store = api.object_store();

        timber::reporting_level = log_level;

        if (daemon && !dmon::daemonize({
            .group = settings.daemon.group,
            .identifier = app.name,
            .pidfile = settings.daemon.pidfile,
            .user = settings.daemon.user
        })) return;

        NOTICE() << app.name << " version " << app.version << " starting up";

        const auto info = fstore::server::server_info {
            .version = std::string(app.version)
        };

        fstore::server::listen(store, info, settings.server, []() {
            INFO() << "Server started. Listening for connections...";
        });

        NOTICE() << app.name << " shutting down";
    }
}

namespace fstore::cli {
    using namespace commline;

    auto start(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "start",
            "Start the server",
            options(
                opts::config(confpath),
                flag(
                    {"d", "daemon"},
                    "Run the program as a daemon"
                ),
                opts::log_level()
            ),
            arguments(),
            $start
        );
    }
}
