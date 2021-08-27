#include "commands.h"

#include <fstore/cli.h>
#include <fstore/core/object_store.h>

#include <ext/data_size.h>
#include <iostream>

static auto $status(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view confpath,
    bool verbose
) -> void {
    const auto settings = fstore::conf::settings::load_file(confpath);
    auto store = fstore::core::object_store(
        settings.database.connection.str(),
        settings.objects_dir
    );

    auto table = fstore::cli::bucket_table();

    auto bucket_names = std::vector<std::string>();
    for (const auto& arg : argv) bucket_names.push_back(std::string(arg));

    auto buckets = verbose ?
        store.fetch_buckets() : store.fetch_buckets(bucket_names);

    for (auto&& bucket : buckets) table.push_back(std::move(bucket));

    if (!table.empty()) std::cout << '\n' << table << '\n';

    if (argv.empty() || verbose) {
        auto totals = store.fetch_store_totals();

        std::cout
            << "buckets: " << totals.buckets << '\n'
            << "objects: " << totals.objects << '\n'
            << "space used: "
            << ext::data_size::format(totals.space_used).str(2)
            << std::endl;
    }
}

namespace fstore::cli {
    using namespace commline;

    auto status(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "status",
            "Print information about the object store.",
            options(
                option<std::string_view>(
                    {"conf", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                ),
                flag(
                    {"verbose", "v"},
                    "Print information about all buckets."
                )
            ),
            $status
        );
    }
}
