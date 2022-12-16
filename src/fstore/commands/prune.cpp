#include "commands.hpp"
#include "../api/api.hpp"
#include "../options/opts.hpp"

#include <ext/data_size.h>
#include <iostream>
#include <numeric>

using namespace commline;

namespace {
    auto $prune(
        const app& app,
        std::string_view confpath
    ) -> void {
        auto api = fstore::cli::api_container(confpath);
        auto& store = api.object_store();

        const auto objects = store.prune();

        if (objects.empty()) {
            std::cout << "No objects to prune." << std::endl;
            return;
        }

        const auto space_freed = std::accumulate(
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
            << "freeing " << ext::data_size::format(space_freed).str(2)
            << std::endl;
    }
}

namespace fstore::cli {
    auto prune(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "prune",
            "Remove any objects not referenced by a bucket",
            options(
                opts::config(confpath)
            ),
            arguments(),
            $prune
        );
    }
}
