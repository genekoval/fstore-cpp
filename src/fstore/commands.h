#pragma once

#include <commline/commline>

namespace fstore::cli {
    auto bucket() -> std::unique_ptr<commline::command_node>;
    auto bucket_add() -> std::unique_ptr<commline::command_node>;
    auto bucket_remove() -> std::unique_ptr<commline::command_node>;
    auto bucket_rename() -> std::unique_ptr<commline::command_node>;

    auto object() -> std::unique_ptr<commline::command_node>;
    auto object_add() -> std::unique_ptr<commline::command_node>;
    auto object_remove() -> std::unique_ptr<commline::command_node>;

    auto prune() -> std::unique_ptr<commline::command_node>;
    auto status() -> std::unique_ptr<commline::command_node>;
}
