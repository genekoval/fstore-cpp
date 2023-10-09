#include "../api/api.hpp"
#include "../options/opts.hpp"
#include "commands.hpp"

#include <ext/data_size.h>
#include <iostream>
#include <numeric>

using namespace commline;

namespace {
    namespace internal {
        auto prune(const app& app, std::string_view confpath) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            fstore::cli::object_store(
                settings,
                [&](fstore::core::object_store& store) -> ext::task<> {
                    const auto objects = co_await store.prune();

                    if (objects.empty()) {
                        std::cout << "No objects to prune." << std::endl;
                        co_return;
                    }

                    const auto space_freed = std::accumulate(
                        objects.begin(),
                        objects.end(),
                        uintmax_t(0),
                        [](uintmax_t sum, const auto& object) -> uintmax_t {
                            return sum + object.size;
                        }
                    );

                    std::cout << "removed " << objects.size() << " object"
                              << (objects.size() == 1 ? "" : "s") << '\n'
                              << "freeing "
                              << ext::data_size::format(space_freed).str(2)
                              << std::endl;
                }
            );
        }
    }
}

namespace fstore::cli {
    auto prune(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            "prune",
            "Remove any objects not referenced by a bucket",
            options(opts::config(confpath)),
            arguments(),
            internal::prune
        );
    }
}
