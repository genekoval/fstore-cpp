#include "commands.h"
#include "../options/opts.h"

#include <fstore/cli.h>
#include <fstore/core/object_store.h>

#include <ext/data_size.h>
#include <iostream>

using namespace commline;

namespace {
    auto $status(
        const app& app,
        std::string_view confpath,
        bool verbose,
        const std::vector<std::string>& names
    ) -> void {
        const auto settings = fstore::conf::settings::load_file(confpath);
        auto store = fstore::core::object_store(
            settings.database.connection.str(),
            settings.home
        );

        auto table = fstore::cli::bucket_table();

        auto buckets = verbose ?
            store.fetch_buckets() : store.fetch_buckets(names);

        for (auto&& bucket : buckets) table.push_back(std::move(bucket));

        if (!table.empty()) std::cout << '\n' << table << '\n';

        if (names.empty() || verbose) {
            auto totals = store.fetch_store_totals();

            std::cout
                << "buckets: " << totals.buckets << '\n'
                << "objects: " << totals.objects << '\n'
                << "space used: "
                << ext::data_size::format(totals.space_used).str(2)
                << std::endl;
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
            $status
        );
    }
}
