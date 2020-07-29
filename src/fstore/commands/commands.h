#pragma once

#include <fstore/service.h>

#include <commline/commline>

namespace fstore::cli {
    auto bucket() -> std::unique_ptr<commline::command_node>;
    auto object() -> std::unique_ptr<commline::command_node>;
    auto prune() -> std::unique_ptr<commline::command_node>;
    auto start(
        const service::settings& settings
    ) -> std::unique_ptr<commline::command_node>;
    auto status() -> std::unique_ptr<commline::command_node>;
}
