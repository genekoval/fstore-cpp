#include "commands.h"

#include <fstore/service/object_store.h>

#include <ext/string.h>
#include <iostream>

static auto $bucket(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view confpath
) {
    const auto settings = fstore::conf::settings::load_file(confpath);
    auto store = fstore::service::object_store(
        settings.database.connection.str(),
        settings.objects_dir
    );
    const auto buckets = store.fetch_buckets();
    for (const auto& bucket : buckets) std::cout << bucket.name << '\n';
}

static auto $add(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view confpath
) -> void {
    if (argv.empty()) {
        throw commline::cli_error("No bucket name given.");
    }

    const auto settings = fstore::conf::settings::load_file(confpath);
    auto store = fstore::service::object_store(
        settings.database.connection.str(),
        settings.objects_dir
    );

    const auto bucket = store.create_bucket(argv[0]);
    std::cout << "bucket created: " << bucket.name << std::endl;
}

static auto $remove(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view confpath
) -> void {
    if (argv.empty()) {
        throw commline::cli_error("No bucket name given.");
    }

    const auto settings = fstore::conf::settings::load_file(confpath);
    auto store = fstore::service::object_store(
        settings.database.connection.str(),
        settings.objects_dir
    );

    const auto bucket = store.fetch_bucket(argv[0]);

    store.remove_bucket(bucket.id);

    std::cout << "bucket removed: " << bucket.name << std::endl;
}

static auto $rename(
    const commline::app& app,
    const commline::argv& argv,
    std::string_view confpath
) -> void {
    if (argv.size() < 2) {
        throw commline::cli_error("Old and new bucket name required.");
    }

    const auto settings = fstore::conf::settings::load_file(confpath);
    auto store = fstore::service::object_store(
        settings.database.connection.str(),
        settings.objects_dir
    );

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
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Create a new bucket.",
            options(
                option<std::string_view>(
                    {"conf", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $add
        );
    }

    auto remove(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "remove",
            "Delete a bucket.",
            options(
                option<std::string_view>(
                    {"conf", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $remove
        );
    }

    auto rename(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "rename",
            "Rename a bucket.",
            options(
                option<std::string_view>(
                    {"conf", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $rename
        );
    }

    auto bucket(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        auto cmd = command(
            "bucket",
            "List bucket information",
            options(
                option<std::string_view>(
                    {"conf", "c"},
                    "Path to configuration file",
                    "path",
                    std::move(confpath)
                )
            ),
            $bucket
        );

        cmd->subcommand(add(confpath));
        cmd->subcommand(remove(confpath));
        cmd->subcommand(rename(confpath));

        return cmd;
    }
}
