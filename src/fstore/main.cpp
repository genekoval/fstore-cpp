#include "client/api.h"
#include "commands/commands.h"

#include <fstore/service.h>

#include <commline/commline>

static auto $main(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view unix_socket
) -> void {
    try {
        auto api = fstore::api(unix_socket);

        auto totals = api.get_store_totals();

        std::cout
            << "Bucket count: " << totals.bucket_count << '\n'
            << "Object count: " << totals.object_count << '\n'
            << "Space used: " << totals.space_used
            << std::endl;
    }
    catch (const std::exception& ex) {
        ERROR() << ex.what();
    }
}

auto main(int argc, const char** argv) -> int {
    using namespace commline;

    timber::reporting_level() = timber::level::warn;

    const auto settings = fstore::service::settings();

    auto app = application(
        "fstore",
        "0.1.0",
        "Object storage.",
        options(
            option<std::string_view>(
                {"unix-socket", "u"},
                "Endpoint to listen for connections on.",
                "endpoint",
                settings.unix_socket
            )
        ),
        $main
    );

    app.subcommand(fstore::cli::bucket());
    app.subcommand(fstore::cli::object());
    app.subcommand(fstore::cli::prune());
    app.subcommand(fstore::cli::start(settings));
    app.subcommand(fstore::cli::status());

    return app.run(argc, argv);
}
