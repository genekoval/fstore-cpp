#pragma once

#include <fstore/service/settings.h>

#include <commline/commline>

namespace fstore::cli {
    auto bucket(
        const service::settings& settings
    ) -> std::unique_ptr<commline::command_node>;

    auto prune(
        const service::settings& settings
    ) -> std::unique_ptr<commline::command_node>;

    auto start(
        const service::settings& settings
    ) -> std::unique_ptr<commline::command_node>;

    auto status(
        const service::settings& settings
    ) -> std::unique_ptr<commline::command_node>;
}
