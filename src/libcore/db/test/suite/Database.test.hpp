#pragma once

#include <internal/core/db/database.hpp>

#include <gtest/gtest.h>

class DatabaseTest : public testing::Test {
    auto truncate(std::string_view table) -> ext::task<>;

    auto truncate_tables() -> ext::task<>;
protected:
    static auto connect() -> ext::task<pg::client>;

    static auto database() -> fstore::core::db::database;

    pg::client* client = nullptr;
    fstore::core::db::connection_wrapper* connection = nullptr;
    fstore::core::db::database* db = nullptr;

    auto count(std::string_view table) -> ext::task<std::int64_t>;

    auto run(ext::task<>&& task) -> void;

    virtual auto tables() -> std::vector<std::string_view> = 0;
};
