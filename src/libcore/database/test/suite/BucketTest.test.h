#pragma once

#include "Database.test.h"

class BucketTest : public DatabaseTest {
protected:
    auto create_bucket(std::string_view name) -> fstore::core::db::bucket;

    virtual auto tables() -> std::vector<std::string> override;
};
