#include "commands.h"

#include <fstore/cli.h>
#include <fstore/service/object_store.h>

#include <ext/data_size.h>
#include <iostream>

static auto $status(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view connection_string,
    std::string_view objects_dir,
    bool verbose
) -> void {
    auto store = fstore::service::object_store(connection_string, objects_dir);
    auto table = fstore::cli::bucket_table();

    auto buckets = verbose ?
        store.fetch_buckets() : store.fetch_buckets(argv);

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
        const service::settings& settings
    ) -> std::unique_ptr<command_node> {
        return command(
            "status",
            "Print information about the object store.",
            options(
                option<std::string_view>(
                    {"database"},
                    "Database connection string.",
                    "connection",
                    settings.connection_string
                ),
                option<std::string_view>(
                    {"objects"},
                    "Path to object files.",
                    "objects directory",
                    settings.objects_dir
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
