#include "commands.hpp"

#include "../api/api.hpp"
#include "../options/opts.hpp"

using namespace commline;

namespace {
    namespace internal {
        auto errors(const app& app, std::string_view confpath) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            fstore::cli::object_store(
                settings,
                [&](fstore::core::object_store& store) -> ext::task<> {
                    const auto errors = co_await store.get_errors();

                    for (const auto& error : errors) {
                        fmt::print("{}  {}\n", error.id, error.message);
                    }

                    fmt::print(
                        "{} error{}\n",
                        errors.size(),
                        errors.size() == 1 ? "" : "s"
                    );
                }
            );
        }
    }
}

namespace fstore::cli {
    auto errors(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Get object errors",
            options(opts::config(confpath)),
            arguments(),
            internal::errors
        );
    }
}
