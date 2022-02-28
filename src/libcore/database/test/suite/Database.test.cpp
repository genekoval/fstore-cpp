#include "Database.test.h"

#include <fmt/format.h>

auto DatabaseTest::SetUp() -> void {
    truncate_tables();
}

auto DatabaseTest::count(std::string_view table) -> int {
    return exec1<int>(fmt::format("SELECT count(*) FROM data.{}", table));
}

auto DatabaseTest::truncate(std::string_view table) -> void {
    exec0(fmt::format("TRUNCATE data.{} cascade", table));
}

auto DatabaseTest::truncate_tables() -> void {
    for (const auto& table : tables()) truncate(table);
}
