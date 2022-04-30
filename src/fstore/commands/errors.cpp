#include "commands.h"

#include "../api/api.h"
#include "../options/opts.h"

using namespace commline;

namespace {
    namespace internal {
        auto errors(
            const app& app,
            std::string_view confpath
        ) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            auto api = fstore::cli::api_container(settings);
            auto& store = api.object_store();

            const auto errors = store.get_errors();

            for (const auto& error : errors) {
                fmt::print("{}  {}\n", error.id, error.message);
            }

            fmt::print(
                "{} error{}\n",
                errors.size(),
                errors.size() == 1 ? "" : "s"
            );
        }
    }
}

namespace fstore::cli {
    auto errors(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Get object errors",
            options(
                opts::config(confpath)
            ),
            arguments(),
            internal::errors
        );
    }
}
