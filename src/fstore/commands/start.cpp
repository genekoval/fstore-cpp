#include "commands.hpp"
#include "../api/api.hpp"
#include "../options/opts.hpp"

#include <internal/cli.hpp>
#include <internal/server/server.hpp>
#include <internal/server/http/server.hpp>

#include <dmon/dmon>
#include <timber/timber>

namespace fs = std::filesystem;

using fstore::server::server_list;

namespace {
    namespace internal {
        constexpr auto signals = std::array { SIGINT, SIGTERM };

        auto default_pidfile(std::string_view name) -> fs::path {
            auto path = fs::temp_directory_path() / name;
            path += ".pid";
            return path;
        }

        auto handle_signals(
            server_list& servers,
            http::server::server_list& http_servers
        ) -> ext::jtask<> {
            auto signalfd = netcore::signalfd::create(signals);

            while (true) {
                const auto signal = co_await signalfd.wait_for_signal();

                TIMBER_INFO(
                    "Received signal ({}): {}",
                    signal,
                    strsignal(signal)
                );

                servers.close();
                http_servers.close();
            }
        }

        auto start(
            const commline::app& app,
            std::string_view conf,
            bool daemon,
            std::optional<fs::path> pidfile,
            timber::level log_level
        ) -> void {
            auto startup_timer = timber::timer(
                "Server started in",
                timber::level::info
            );

            const auto settings = fstore::conf::settings::load_file(conf);
            timber::reporting_level = log_level;

            if (daemon && !dmon::daemonize({
                .group = settings.daemon.group,
                .identifier = app.name,
                .pidfile = pidfile.value_or(default_pidfile(app.name)),
                .user = settings.daemon.user
            })) return;

            TIMBER_NOTICE("{} version {} starting up", app.name, app.version);

            fstore::cli::object_store(settings, [&](
                fstore::core::object_store& store
            ) -> ext::task<> {
                const auto info = fstore::server_info {
                    .version = std::string(app.version)
                };

                auto routes = fstore::server::http::router(info, store);
                auto http = co_await fstore::server::http::listen(
                    routes,
                    settings.http.cert,
                    settings.http.key,
                    settings.http.listen
                );

                auto router = fstore::server::make_router(store, info);
                auto servers = co_await fstore::server::listen(
                    router,
                    settings.server
                );

                startup_timer.stop();
                auto uptime_timer = timber::timer(
                    "Server shutting down. Up",
                    timber::level::notice
                );

                const auto sigtask = handle_signals(servers, http);
                co_await servers.join();
                co_await http.join();

                uptime_timer.stop();
            });
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
                option<std::optional<fs::path>>(
                    {"p", "pidfile"},
                    "Path to the pidfile",
                    "path"
                ),
                opts::log_level()
            ),
            arguments(),
            internal::start
        );
    }
}
