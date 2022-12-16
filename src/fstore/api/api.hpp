#pragma once

#include <internal/conf/settings.hpp>
#include <internal/core/db/database.hpp>
#include <internal/core/fs/filesystem.hpp>
#include <internal/core/object_store.hpp>

namespace fstore::cli {
    class api_container {
        fstore::core::db::database database;
        fstore::core::fs::filesystem fs;
        fstore::core::object_store store;
    public:
        api_container(std::string_view confpath);

        api_container(const fstore::conf::settings& config);

        auto object_store() -> core::object_store&;
    };
}
