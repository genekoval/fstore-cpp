#include "commands.h"

#include <commline/commline>

static auto $main(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    throw commline::cli_error("Not implemented.");
}

auto main(int argc, const char** argv) -> int {
    auto app = commline::application(
        "fstore",
        "0.1.0",
        "Object storage.",
        $main
    );

    app.subcommand(fstore::cli::bucket());
    app.subcommand(fstore::cli::object());
    app.subcommand(fstore::cli::prune());
    app.subcommand(fstore::cli::status());

    return app.run(argc, argv);
}
