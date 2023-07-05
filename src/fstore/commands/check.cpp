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
        constexpr auto refresh_rate = 200ms;

        auto print_progress(const check_progress& progress) -> void {
            fmt::print(stderr, "\33[2K\033[A\33[2K\r");

            if (progress.total == 0) return;

            const auto completed = progress.success + progress.errors;

            const auto percent = static_cast<int>(std::round(
                static_cast<long double>(completed) /
                static_cast<long double>(progress.total) * 100
            ));

            fmt::print(
                stderr,
                "[{:L}/{:L}] ({}%)\n",
                completed,
                progress.total,
                percent
            );
        }

        auto run_progress_printer(
            const check_progress& progress,
            const ext::task<>& task
        ) -> ext::jtask<> {
            while (!task.is_ready()) {
                co_await netcore::sleep_for(refresh_rate);
                print_progress(progress);
            }
        }

        auto check(
            const app& app,
            std::string_view confpath,
            int batch_size,
            int jobs,
            bool quiet
        ) -> void {
            jobs = std::max(jobs, 1);

            auto settings = fstore::conf::settings::load_file(confpath);

            fmt::print(
                "Scanning object files using up to {} additional threads...\n",
                jobs
            );

            if (!quiet) fmt::print("\n");

            auto progress = check_progress();

            fstore::cli::object_store(settings, [&](
                fstore::core::object_store& store
            ) -> ext::task<> {
                const auto task = store.check(batch_size, jobs, progress);
                const auto printer = quiet ? ext::jtask<>() :
                    run_progress_printer(progress, task);

                co_await task;
                if (printer.joinable()) co_await printer;
            });

            const auto completed = progress.success + progress.errors;

            fmt::print(
                "Checked {:L} object{}: {} valid",
                completed,
                completed == 1 ? "" : "s",
                progress.errors == 0 ? "all" :
                    fmt::format("{:L}", progress.success)
            );

            if (progress.errors > 0) {
                fmt::print(
                    ", {:L} error{}\n",
                    progress.errors,
                    progress.errors == 1 ? "" : "s"
                );
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
                    {"b", "batch-size"},
                    "Number of objects to fetch from the database at a time",
                    "objects",
                    500
                ),
                option<int>(
                    {"j", "jobs"},
                    "Number of jobs to run simultaneously",
                    "jobs",
                    1
                ),
                flag({"q", "quiet"}, "Do not print progress")
            ),
            arguments(),
            internal::check
        );
    }
}
