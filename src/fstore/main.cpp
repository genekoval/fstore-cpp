#include "commands/commands.h"

#include <fstore/service/settings.h>

#include <commline/commline>
#include <timber/timber>

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
    using namespace commline;

    timber::reporting_level() = timber::level::warn;

    const auto settings = fstore::service::settings();

    auto app = application(
        NAME,
        VERSION,
        DESCRIPTION,
        options(
            flag(
                {"version", "v"},
                "Print the program version information."
            )
        ),
        $main
    );

    app.subcommand(fstore::cli::bucket(settings));
    app.subcommand(fstore::cli::prune(settings));
    app.subcommand(fstore::cli::start(settings));
    app.subcommand(fstore::cli::status(settings));

    return app.run(argc, argv);
}
