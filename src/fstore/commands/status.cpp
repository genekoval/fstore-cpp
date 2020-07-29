#include "commands.h"

#include <fstore/cli.h>

#include <ext/data_size.h>
#include <iostream>

static auto $status(
    const commline::app& app,
    const commline::argv& argv,
    bool verbose
) -> void {
    const auto object_store = fstore::object_store();
    auto table = fstore::bucket_table();

    auto sargv = std::vector<std::string>();
    for (const auto arg : argv) sargv.push_back(std::string(arg));

    auto buckets = std::vector<std::unique_ptr<fstore::core::bucket>>();
    if (verbose) buckets = object_store->fetch_buckets();
    else if (!argv.empty()) buckets = object_store->fetch_buckets(sargv);

    for (auto&& bucket : buckets) table.push_back(std::move(bucket));

    if (!table.empty()) std::cout << '\n' << table << '\n';

    if (argv.empty() || verbose) {
        auto totals = object_store->get_store_totals();

        std::cout
            << "Buckets: " << totals->bucket_count() << '\n'
            << "Objects: " << totals->object_count() << '\n'
            << "Space Used: " << ext::data_size::format(totals->space_used())
            << '\n';
    }
}

namespace fstore::cli {
    using namespace commline;

    auto status() -> std::unique_ptr<command_node> {
        return command(
            "status",
            "Print information about the object store.",
            options(
                flag(
                    {"verbose", "v"},
                    "Print information about all buckets."
                )
            ),
            $status
        );
    }
}
