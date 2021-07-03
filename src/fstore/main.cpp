#include "commands/commands.h"

#include <fstore/conf/settings.h>

#include <commline/commline>
#include <timber/timber>

const auto default_config = std::filesystem::path(CONFDIR) / "fstore.yml";

static auto $main(
    const commline::app& app,
    const commline::argv& argv,
    bool version
) -> void {
    if (version) {
        commline::print_version(std::cout, app);
        return;
    }
}

auto main(int argc, const char** argv) -> int {
    const auto confpath = default_config.string();

    auto app = commline::application(
        NAME,
        VERSION,
        DESCRIPTION,
        commline::options(
            commline::flag(
                {"version", "v"},
                "Print the program version information."
            )
        ),
        $main
    );

    app.subcommand(fstore::cli::bucket(confpath));
    app.subcommand(fstore::cli::prune(confpath));
    app.subcommand(fstore::cli::start(confpath));
    app.subcommand(fstore::cli::status(confpath));

    return app.run(argc, argv);
}
