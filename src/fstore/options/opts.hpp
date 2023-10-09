#pragma once

#include <commline/commline>
#include <timber/timber>

namespace fstore::cli::opts {
    auto config(std::string_view confpath)
        -> commline::option<std::string_view>;

    auto log_level() -> commline::option<timber::level>;
}
