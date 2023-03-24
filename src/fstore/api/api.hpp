#pragma once

#include <internal/conf/settings.hpp>
#include <internal/core/db/database.hpp>
#include <internal/core/fs/filesystem.hpp>
#include <internal/core/object_store.hpp>

namespace fstore::cli {
    class api_container {
        pg::parameters params;
        core::db::database database;
        core::fs::filesystem fs;
        core::object_store store;
    public:
        api_container(std::string_view confpath);

        api_container(const conf::settings& config);

        auto object_store() -> core::object_store&;
    };

    template <typename F>
    requires requires(const F& f, core::object_store& object_store) {
        { f(object_store) } -> std::same_as<ext::task<>>;
    }
    auto object_store(
        const conf::settings& settings,
        const F& action
    ) -> void {
        netcore::async([&]() -> ext::task<> {
            auto container = api_container(settings);
            co_await action(container.object_store());
        }());
    }
}
