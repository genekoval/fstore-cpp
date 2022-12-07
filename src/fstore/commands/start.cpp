#include "commands.h"
#include "../api/api.h"
#include "../options/opts.h"

#include <fstore/cli.h>
#include <fstore/core/object_store.h>
#include <fstore/net/zipline/coder.h>
#include <fstore/server/server.h>

#include <dmon/dmon>
#include <timber/timber>

namespace {
    namespace internal {
        auto start(
            const commline::app& app,
            std::string_view conf,
            bool daemon,
            timber::level log_level
        ) -> void {
            auto startup_timer = timber::timer(
                "Server started in",
                timber::level::info
            );

            auto uptime_timer = timber::timer(
                "Server shutting down. Up",
                timber::level::notice
            );

            const auto settings = fstore::conf::settings::load_file(conf);
            timber::reporting_level = log_level;

            if (daemon && !dmon::daemonize({
                .group = settings.daemon.group,
                .identifier = app.name,
                .pidfile = settings.daemon.pidfile,
                .user = settings.daemon.user
            })) return;

            TIMBER_NOTICE("{} version {} starting up", app.name, app.version);

            netcore::async([&]() -> ext::task<> {
                auto api = fstore::cli::api_container(settings);
                auto& store = api.object_store();

                const auto info = fstore::server::server_info {
                    .version = std::string(app.version)
                };

                auto server = fstore::server::create(
                    store,
                    info,
                    startup_timer,
                    uptime_timer
                );

                co_await server.listen(settings.server);
            }());
        }
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
            internal::start
        );
    }
}
