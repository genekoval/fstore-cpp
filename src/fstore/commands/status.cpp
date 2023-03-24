#include "commands.hpp"
#include "../api/api.hpp"
#include "../options/opts.hpp"

#include <internal/cli.hpp>

#include <ext/data_size.h>
#include <iostream>

using namespace commline;

namespace {
    namespace internal {
        auto status(
            const app& app,
            std::string_view confpath,
            bool verbose,
            const std::vector<std::string>& names
        ) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            fstore::cli::object_store(settings, [&](
                fstore::core::object_store& store
            ) -> ext::task<> {
                auto table = fstore::cli::bucket_table();

                auto buckets = verbose ?
                    co_await store.fetch_buckets() :
                    co_await store.fetch_buckets(names);

                for (auto&& bucket : buckets) {
                    table.push_back(std::forward<fstore::bucket>(bucket));
                }

                if (!table.empty()) std::cout << '\n' << table << '\n';

                if (names.empty() || verbose) {
                    const auto totals = co_await store.fetch_store_totals();

                    std::cout
                        << "buckets: " << totals.buckets << '\n'
                        << "objects: " << totals.objects << '\n'
                        << "space used: "
                        << ext::data_size::format(totals.space_used).str(2)
                        << std::endl;
                }
            });
        }
    }
}

namespace fstore::cli {
    auto status(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "status",
            "Print information about the object store",
            options(
                opts::config(confpath),
                flag(
                    {"v", "verbose"},
                    "Print information about all buckets"
                )
            ),
            arguments(
                variadic<std::string>("names")
            ),
            internal::status
        );
    }
}
