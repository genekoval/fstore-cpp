#pragma once

#include <fstore/core/db/database.h>

#include <filesystem>

namespace fstore::test {
    struct temp_directory {
        const std::filesystem::path path;

        temp_directory();
        ~temp_directory();
    };

    auto db() -> core::db::database;

    auto create_bucket(
        core::db::database& db,
        std::string_view name
    ) -> core::db::bucket;

    auto drop_buckets() -> void;

    auto drop_objects() -> void;

    auto start_server(const std::filesystem::path& unix_socket) -> pid_t;

    auto stop_server(pid_t pid) -> void;
}
