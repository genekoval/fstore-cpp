#pragma once

#include <fstore/repo/db.h>

namespace fstore::test {
    auto db() -> repo::db;

    auto create_bucket(repo::db& db, std::string_view name) -> model::bucket;

    auto drop_buckets() -> void;

    auto drop_objects() -> void;
}
