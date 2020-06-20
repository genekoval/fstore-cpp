#include "cli.h"
#include "commands.h"

#include <ext/string.h>

#include <iostream>

static auto $bucket(
    const commline::app& app,
    const commline::argv& argv
) {
    const auto buckets = fstore::object_store()->fetch_buckets();
    for (const auto& bucket : buckets) std::cout << bucket->name() << '\n';
}

static auto $add(const commline::app& app, const commline::argv& argv) -> void {
    if (argv.empty()) {
        throw commline::cli_error("No bucket name given.");
    }

    const auto& name = argv[0];

    const auto bucket =
        fstore::object_store()->create_bucket(std::string(name));
    std::cout << "Created bucket: " << bucket->name() << std::endl;
}

static auto $remove(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.empty()) {
        throw commline::cli_error("No bucket name given.");
    }

    const auto& name = argv[0];

    auto bucket = fstore::fetch_bucket(name);
    bucket->drop();
    std::cout << "Deleted bucket: " << bucket->name() << std::endl;
}

static auto $rename(
    const commline::app& app,
    const commline::argv& argv
) -> void {
    if (argv.size() < 2) {
        throw commline::cli_error("Old and new bucket name required.");
    }

    auto bucket = fstore::fetch_bucket(argv[0]);
    // Create a local copy.
    // Once the bucket is renamed, the sanitized old name will be unreachable.
    const auto old_name = std::string(bucket->name());

    bucket->name(std::string(argv[1]));

    std::cout
        << "bucket " QUOTE(old_name) << " renamed to: " << bucket->name()
        << std::endl;
}

namespace fstore::cli {
    auto bucket() -> std::unique_ptr<commline::command_node> {
        auto cmd = commline::command(
            "bucket",
            "List bucket information",
            $bucket
        );

        cmd->subcommand(bucket_add());
        cmd->subcommand(bucket_remove());
        cmd->subcommand(bucket_rename());

        return cmd;
    }

    auto bucket_add() -> std::unique_ptr<commline::command_node> {
        return commline::command(
            "add",
            "Create a new bucket.",
            $add
        );
    }

    auto bucket_remove() -> std::unique_ptr<commline::command_node> {
        return commline::command(
            "remove",
            "Delete a bucket.",
            $remove
        );
    }

    auto bucket_rename() -> std::unique_ptr<commline::command_node> {
        return commline::command(
            "rename",
            "Rename a bucket.",
            $rename
        );
    }
}
