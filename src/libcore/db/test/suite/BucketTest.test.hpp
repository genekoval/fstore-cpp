#pragma once

#include "Database.test.hpp"

class BucketTest : public DatabaseTest {
protected:
    virtual auto tables() -> std::vector<std::string_view> override;
};
