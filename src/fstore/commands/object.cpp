#include "commands.h"

#include <fstore/cli.h>
#include <fstore/error.h>

#include <ext/data_size.h>
#include <ext/string.h>

static auto $add(const commline::app& app, const commline::argv& argv) -> void {
    if (argv.empty()) {
        throw commline::cli_error("Missing bucket name.");
    }

    auto bucket = fstore::fetch_bucket(argv[0]);

    if (argv.size() == 1) {
        throw commline::cli_error("No objects to add.");
    }

    for (std::size_t i = 1; i < argv.size(); i++) {
        const auto& file = argv[i];
        auto object = bucket->add_object(file);
        std::cout << object->id() << " " << file << '\n';
    }

    std::cout << std::flush;
}

static auto $remove(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.empty()) {
        throw commline::cli_error("Missing bucket name.");
    }

    auto bucket = fstore::fetch_bucket(argv[0]);

    for (std::size_t i = 1; i < argv.size(); i++) {
        const auto& id = argv[i];

        try {
            auto object = bucket->remove_object(std::string(id));
            std::cout
                << "Removed: "
                << object->id() << " "
                << object->hash() << " "
                << ext::data_size::format(object->size())
                << '\n';
        }
        catch (const fstore::fstore_error& ex) {
            std::cout << "skipped: " << id << ": " << ex.what() << '\n';
        }
    }

    std::cout << std::flush;
}

namespace fstore::cli {
    auto object_add() -> std::unique_ptr<commline::command_node> {
        return commline::command(
            "add",
            "Add a file to a bucket.",
            $add
        );
    }

    auto object_remove() -> std::unique_ptr<commline::command_node> {

        return commline::command(
            "remove",
            "Remove an object from a bucket.",
            $remove
        );
    }

    auto object() -> std::unique_ptr<commline::command_node> {
        auto cmd = commline::command(
            "object",
            "Not implemented.",
            [](const commline::app& app, const commline::argv& argv) {}
        );

        cmd->subcommand(object_add());
        cmd->subcommand(object_remove());

        return cmd;
    }
}
