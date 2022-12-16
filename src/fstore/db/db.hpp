#pragma once

#include <internal/conf/settings.hpp>

#include <dbtools/dbtools>

namespace fstore::cli {
    auto database(const conf::settings& settings) -> dbtools::postgresql;

    auto dump_file(const conf::settings& settings) -> std::string;
}
