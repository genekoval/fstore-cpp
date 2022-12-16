#include "opts.hpp"

using namespace commline;

namespace fstore::cli::opts {
    auto log_level() -> option<timber::level> {
        return option<timber::level>(
            {"l", "log-level"},
            "Minimum log level to display",
            "level",
            timber::level::info
        );
    }
}
