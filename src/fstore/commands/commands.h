#pragma once

#include <fstore/conf/settings.h>

#include <commline/commline>

namespace fstore::cli {
    auto archive(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto bucket(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto check(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto db(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto errors(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto init(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto migrate(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto prune(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto restore(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto start(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;

    auto status(
        std::string_view confpath
    ) -> std::unique_ptr<commline::command_node>;
}
