#include "../api/api.hpp"
#include "../options/opts.hpp"
#include "commands.hpp"

#include <ext/string.h>
#include <iostream>

namespace {
    namespace internal {
        auto bucket(const commline::app& app, std::string_view confpath) {
            const auto settings = fstore::conf::settings::load_file(confpath);
            fstore::cli::object_store(
                settings,
                [&](fstore::core::object_store& store) -> ext::task<> {
                    const auto buckets = co_await store.fetch_buckets();
                    for (const auto& bucket : buckets) {
                        std::cout << bucket.name << '\n';
                    }
                }
            );
        }

        auto add(
            const commline::app& app,
            std::string_view confpath,
            std::string_view name
        ) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            fstore::cli::object_store(
                settings,
                [&](fstore::core::object_store& store) -> ext::task<> {
                    const auto bucket = co_await store.create_bucket(name);
                    std::cout << "bucket created: " << bucket.name << std::endl;
                }
            );
        }

        auto remove(
            const commline::app& app,
            std::string_view confpath,
            std::string_view name
        ) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            fstore::cli::object_store(
                settings,
                [&](fstore::core::object_store& store) -> ext::task<> {
                    const auto bucket = co_await store.fetch_bucket(name);

                    co_await store.remove_bucket(bucket.id);

                    std::cout << "bucket removed: " << bucket.name << std::endl;
                }
            );
        }

        auto rename(
            const commline::app& app,
            std::string_view confpath,
            std::string_view old_name,
            std::string_view new_name
        ) -> void {
            const auto settings = fstore::conf::settings::load_file(confpath);
            fstore::cli::object_store(
                settings,
                [&](fstore::core::object_store& store) -> ext::task<> {
                    const auto old = co_await store.fetch_bucket(old_name);
                    co_await store.rename_bucket(old.id, new_name);

                    const auto bucket = co_await store.fetch_bucket(new_name);

                    std::cout << "bucket " QUOTE(old.name)
                              << " renamed to: " << bucket.name << std::endl;
                }
            );
        }
    }
}

namespace fstore::cli {
    using namespace commline;

    auto add(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            "add",
            "Create a new bucket",
            options(opts::config(confpath)),
            arguments(required<std::string_view>("name")),
            internal::add
        );
    }

    auto remove(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            "remove",
            "Delete a bucket",
            options(opts::config(confpath)),
            arguments(required<std::string_view>("name")),
            internal::remove
        );
    }

    auto rename(std::string_view confpath) -> std::unique_ptr<command_node> {
        return command(
            "rename",
            "Rename a bucket",
            options(opts::config(confpath)),
            arguments(
                required<std::string_view>("old_name"),
                required<std::string_view>("new_name")
            ),
            internal::rename
        );
    }

    auto bucket(std::string_view confpath) -> std::unique_ptr<command_node> {
        auto cmd = command(
            "bucket",
            "List bucket information",
            options(opts::config(confpath)),
            arguments(),
            internal::bucket
        );

        cmd->subcommand(add(confpath));
        cmd->subcommand(remove(confpath));
        cmd->subcommand(rename(confpath));

        return cmd;
    }
}
