#include "commands.h"

#include <fstore/cli.h>
#include <fstore/server/server.h>
#include <fstore/service.h>

#include <timber/timber>
#include <unistd.h>

static auto $start(
    const commline::app& app,
    const commline::argv& argv,
    timber::level log_level,
    std::string_view unix_socket,
    std::string_view db_connection,
    std::string_view objects_dir
) -> void {
    timber::reporting_level() = log_level;

    INFO()
        << app.name
        << " version " << app.version
        << " starting: [PID " << getpid() << ']';

    auto store = fstore::service::connect(db_connection, objects_dir);

    fstore::server::listen(*store, unix_socket);
}

namespace fstore::cli {
    using namespace commline;

    auto start(
        const service::settings& settings
    ) -> std::unique_ptr<command_node> {
        return command(
            "start",
            "Start the server.",
            options(
                option<timber::level>(
                    {"log-level", "l"},
                    "Minimum log level to display.",
                    "log level",
                    timber::level::info
                ),
                option<std::string_view>(
                    {"unix-socket", "u"},
                    "Endpoint to listen for connections on.",
                    "endpoint",
                    settings.unix_socket
                ),
                option<std::string_view>(
                    {"database"},
                    "Database connection string.",
                    "connection",
                    settings.connection_string
                ),
                option<std::string_view>(
                    {"objects"},
                    "Path to object files.",
                    "objects directory",
                    settings.objects_dir
                )
            ),
            $start
        );
    }
}
