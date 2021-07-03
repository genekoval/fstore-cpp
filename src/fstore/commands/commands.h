#pragma once

#include <fstore/conf/settings.h>

#include <commline/commline>

namespace fstore::cli {
    auto bucket(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto prune(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto start(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto status(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
