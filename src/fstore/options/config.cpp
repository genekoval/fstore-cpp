#include "opts.hpp"

using namespace commline;

namespace fstore::cli::opts {
    auto config(
        std::string_view confpath
    ) -> option<std::string_view> {
        return option<std::string_view>(
            {"c", "config"},
            "Path to configuration file",
            "path",
            std::move(confpath)
        );
    }
}
