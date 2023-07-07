#include "commands/commands.hpp"

#include <internal/conf/settings.hpp>

#include <commline/commline>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    namespace internal {
        constexpr auto crash_log_level = timber::level::critical;
        const auto default_config = fs::path(CONFDIR) / "fstore.yml";

        auto main(
            const commline::app& app,
            bool version
        ) -> void {
            if (version) {
                commline::print_version(std::cout, app);
                return;
            }
        }

        auto handle_error(std::exception_ptr eptr) -> void {
            try {
                if (eptr) std::rethrow_exception(eptr);
            }
            catch (const std::exception& ex) {
                TIMBER_LOG(internal::crash_log_level, ex.what());
            }
        }
    }
}

auto main(int argc, char** argv) -> int {
    using namespace commline;

    std::locale::global(std::locale(""));

    timber::thread_name = "main";
    timber::reporting_level = timber::level::warning;
    timber::log_handler = &timber::console::logger;
    timber::set_terminate(internal::crash_log_level);

    const auto confpath = internal::default_config.string();

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
        internal::main
    );

    app.on_error(internal::handle_error);

    app.subcommand(fstore::cli::archive(confpath));
    app.subcommand(fstore::cli::bucket(confpath));
    app.subcommand(fstore::cli::check(confpath));
    app.subcommand(fstore::cli::errors(confpath));
    app.subcommand(fstore::cli::init(confpath));
    app.subcommand(fstore::cli::migrate(confpath));
    app.subcommand(fstore::cli::prune(confpath));
    app.subcommand(fstore::cli::restore(confpath));
    app.subcommand(fstore::cli::start(confpath));
    app.subcommand(fstore::cli::status(confpath));

    return app.run(argc, argv);
}
