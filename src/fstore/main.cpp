#include "commands/commands.h"

#include <fstore/conf/settings.h>

#include <commline/commline>
#include <timber/timber>

namespace {
    const auto default_config = std::filesystem::path(CONFDIR) / "fstore.yml";

    auto initialize_logger() -> void {
        timber::reporting_level = timber::level::warning;
        timber::log_handler = &timber::console_logger;
    }

    auto $main(
        const commline::app& app,
        bool version
    ) -> void {
        if (version) {
            commline::print_version(std::cout, app);
            return;
        }
    }
}

auto main(int argc, const char** argv) -> int {
    using namespace commline;

    initialize_logger();

    const auto confpath = default_config.string();

    auto app = application(
        NAME,
        VERSION,
        DESCRIPTION,
        options(
            flag(
                {"v", "version"},
                "Print the program version information"
            )
        ),
        arguments(),
        $main
    );

    app.on_error([](const auto& e) -> void {
        TIMBER_CRITICAL(e.what());
    });

    app.subcommand(fstore::cli::archive(confpath));
    app.subcommand(fstore::cli::bucket(confpath));
    app.subcommand(fstore::cli::db(confpath));
    app.subcommand(fstore::cli::init(confpath));
    app.subcommand(fstore::cli::migrate(confpath));
    app.subcommand(fstore::cli::prune(confpath));
    app.subcommand(fstore::cli::restore(confpath));
    app.subcommand(fstore::cli::start(confpath));
    app.subcommand(fstore::cli::status(confpath));
    app.subcommand(fstore::cli::stop(confpath));

    return app.run(argc, argv);
}
