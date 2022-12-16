#include "commands.hpp"
#include "../api/api.hpp"
#include "../options/opts.hpp"

#include <chrono>
#include <fmt/format.h>
#include <thread>

using namespace commline;
using namespace std::chrono_literals;

using fstore::core::check_progress;

namespace {
    namespace internal {
        /**
         * One connection for streaming objects and one for logging errors.
         */
        constexpr auto minimum_connections = 2;
        constexpr auto refresh_rate = 500ms;

        bool running = true;

        auto print_progress(const check_progress& progress) -> void {
            std::cerr << "\33[2K\033[A\33[2K\r";

            if (progress.total == 0) return;

            const auto percent = static_cast<int>(std::round(
                static_cast<long double>(progress.completed) /
                static_cast<long double>(progress.total) * 100
            ));

            fmt::print(
                stderr,
                "[{}/{}] ({}%)\n",
                progress.completed,
                progress.total,
                percent
            );
        }

        auto run_progress_printer(const check_progress& progress) -> void {
            while (running) {
                std::this_thread::sleep_for(refresh_rate);
                print_progress(progress);
            }
        }

        auto check(
            const app& app,
            std::string_view confpath,
            int jobs,
            bool quiet
        ) -> void {
            jobs = std::max(jobs, 0);

            auto settings = fstore::conf::settings::load_file(confpath);
            settings.database.connections = jobs + minimum_connections;

            auto api = fstore::cli::api_container(settings);
            auto& store = api.object_store();

            auto progress = check_progress();

            fmt::print(
                "Scanning object files using up to {} additional threads...\n",
                jobs
            );

            if (!quiet) fmt::print("\n");

            auto ui_thread = quiet ?
                std::thread() :
                std::thread(run_progress_printer, std::ref(progress));

            const auto errors = store.check(jobs, progress);

            running = false;
            if (ui_thread.joinable()) ui_thread.join();

            const auto successful = progress.completed - errors;

            fmt::print(
                "Checked {} objects: {} valid",
                progress.completed,
                successful == progress.completed ?
                    "all" :
                    fmt::format("{}", successful)
            );

            if (errors > 0) {
                fmt::print(", {} errors\n", errors);
                fmt::print(
                    "Run '{} errors' for further information\n",
                    app.argv0
                );
            }
            else fmt::print("\n");
        }
    }
}

namespace fstore::cli {
    auto check(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Check integrity of objects",
            options(
                opts::config(confpath),
                option<int>(
                    {"j", "jobs"},
                    "Number of additional jobs to run simultaneously",
                    "jobs",
                    0
                ),
                flag({"q", "quiet"}, "Do not print progress")
            ),
            arguments(),
            internal::check
        );
    }
}
