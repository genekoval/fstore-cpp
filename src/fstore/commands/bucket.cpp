#include "commands.h"

#include <fstore/service/object_store.h>

#include <ext/string.h>
#include <iostream>

static auto $bucket(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view connection_string,
    std::string_view objects_dir
) {
    auto store = fstore::service::object_store(connection_string, objects_dir);
    const auto buckets = store.fetch_buckets();
    for (const auto& bucket : buckets) std::cout << bucket.name << '\n';
}

static auto $add(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view connection_string,
    std::string_view objects_dir
) -> void {
    if (argv.empty()) {
        throw commline::cli_error("No bucket name given.");
    }

    auto store = fstore::service::object_store(connection_string, objects_dir);

    const auto bucket = store.create_bucket(argv[0]);
    std::cout << "bucket created: " << bucket.name << std::endl;
}

static auto $remove(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view connection_string,
    std::string_view objects_dir
) -> void {
    if (argv.empty()) {
        throw commline::cli_error("No bucket name given.");
    }

    auto store = fstore::service::object_store(connection_string, objects_dir);
    const auto bucket = store.fetch_bucket(argv[0]);

    store.remove_bucket(bucket.id);

    std::cout << "bucket removed: " << bucket.name << std::endl;
}

static auto $rename(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view connection_string,
    std::string_view objects_dir
) -> void {
    if (argv.size() < 2) {
        throw commline::cli_error("Old and new bucket name required.");
    }

    auto store = fstore::service::object_store(connection_string, objects_dir);
    const auto old = store.fetch_bucket(argv[0]);
    store.rename_bucket(old.id, argv[1]);

    const auto bucket = store.fetch_bucket(argv[1]);

    std::cout
        << "bucket " QUOTE(old.name)
        << " renamed to: " << argv[1]
        << std::endl;
}

namespace fstore::cli {
    using namespace commline;

    auto add(
        const service::settings& settings
    ) -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Create a new bucket.",
            options(
                option<std::string_view>(
                    {"database"},
                    "Database connection string.",
                    "connection",
                    settings.connection_string
                ),
                option<std::string_view>(
                    {"objects"},
                    "Path to object files.",
                    "objects directory",
                    settings.objects_dir
                )
            ),
            $add
        );
    }

    auto remove(
        const service::settings& settings
    ) -> std::unique_ptr<command_node> {
        return command(
            "remove",
            "Delete a bucket.",
            options(
                option<std::string_view>(
                    {"database"},
                    "Database connection string.",
                    "connection",
                    settings.connection_string
                ),
                option<std::string_view>(
                    {"objects"},
                    "Path to object files.",
                    "objects directory",
                    settings.objects_dir
                )
            ),
            $remove
        );
    }

    auto rename(
        const service::settings& settings
    ) -> std::unique_ptr<command_node> {
        return command(
            "rename",
            "Rename a bucket.",
            options(
                option<std::string_view>(
                    {"database"},
                    "Database connection string.",
                    "connection",
                    settings.connection_string
                ),
                option<std::string_view>(
                    {"objects"},
                    "Path to object files.",
                    "objects directory",
                    settings.objects_dir
                )
            ),
            $rename
        );
    }

    auto bucket(
        const service::settings& settings
    ) -> std::unique_ptr<command_node> {
        auto cmd = command(
            "bucket",
            "List bucket information",
            options(
                option<std::string_view>(
                    {"database"},
                    "Database connection string.",
                    "connection",
                    settings.connection_string
                ),
                option<std::string_view>(
                    {"objects"},
                    "Path to object files.",
                    "objects directory",
                    settings.objects_dir
                )
            ),
            $bucket
        );

        cmd->subcommand(add(settings));
        cmd->subcommand(remove(settings));
        cmd->subcommand(rename(settings));

        return cmd;
    }
}
