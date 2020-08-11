#include "commands.h"

#include <fstore/service/object_store.h>

#include <ext/data_size.h>
#include <iostream>
#include <numeric>

auto $prune(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view connection_string,
    std::string_view objects_dir
) -> void {
    auto store = fstore::service::object_store(connection_string, objects_dir);

    const auto objects = store.prune();

    if (objects.empty()) {
        std::cout << "No objects to prune." << std::endl;
        return;
    }

    auto space_freed = std::accumulate(
        objects.begin(),
        objects.end(),
        uintmax_t(0),
        [](uintmax_t sum, const auto& object) -> uintmax_t {
            return sum + object.size;
        }
    );

    std::cout
        << "removed " << objects.size()
        << " object" << (objects.size() == 1 ? "" : "s") << '\n'
        << "freeing " << ext::data_size::format(space_freed) << std::endl;
}

namespace fstore::cli {
    using namespace commline;

    auto prune(
        const service::settings& settings
    ) -> std::unique_ptr<command_node> {
        return command(
            "prune",
            "Remove any objects not referenced by a bucket.",
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
                )
            ),
            $prune
        );
    }
}
