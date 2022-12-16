#include "commands.hpp"
#include "../api/api.hpp"
#include "../options/opts.hpp"

#include <ext/string.h>
#include <iostream>

namespace {
    auto $bucket(
        const commline::app& app,
        std::string_view confpath
    ) {
        auto api = fstore::cli::api_container(confpath);
        auto& store = api.object_store();

        const auto buckets = store.fetch_buckets();
        for (const auto& bucket : buckets) std::cout << bucket.name << '\n';
    }

    auto $add(
        const commline::app& app,
        std::string_view confpath,
        std::string_view name
    ) -> void {
        auto api = fstore::cli::api_container(confpath);
        auto& store = api.object_store();

        const auto bucket = store.create_bucket(name);
        std::cout << "bucket created: " << bucket.name << std::endl;
    }

    auto $remove(
        const commline::app& app,
        std::string_view confpath,
        std::string_view name
    ) -> void {
        auto api = fstore::cli::api_container(confpath);
        auto& store = api.object_store();

        const auto bucket = store.fetch_bucket(name);

        store.remove_bucket(bucket.id);

        std::cout << "bucket removed: " << bucket.name << std::endl;
    }

    auto $rename(
        const commline::app& app,
        std::string_view confpath,
        std::string_view old_name,
        std::string_view new_name
    ) -> void {
        auto api = fstore::cli::api_container(confpath);
        auto& store = api.object_store();

        const auto old = store.fetch_bucket(old_name);
        store.rename_bucket(old.id, new_name);

        const auto bucket = store.fetch_bucket(new_name);

        std::cout
            << "bucket " QUOTE(old.name)
            << " renamed to: " << bucket.name
            << std::endl;
    }
}

namespace fstore::cli {
    using namespace commline;

    auto add(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Create a new bucket",
            options(
                opts::config(confpath)
            ),
            arguments(
                required<std::string_view>("name")
            ),
            $add
        );
    }

    auto remove(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "remove",
            "Delete a bucket",
            options(
                opts::config(confpath)
            ),
            arguments(
                required<std::string_view>("name")
            ),
            $remove
        );
    }

    auto rename(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            "rename",
            "Rename a bucket",
            options(
                opts::config(confpath)
            ),
            arguments(
                required<std::string_view>("old_name"),
                required<std::string_view>("new_name")
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
                opts::config(confpath)
            ),
            arguments(),
            $bucket
        );

        cmd->subcommand(add(confpath));
        cmd->subcommand(remove(confpath));
        cmd->subcommand(rename(confpath));

        return cmd;
    }
}
