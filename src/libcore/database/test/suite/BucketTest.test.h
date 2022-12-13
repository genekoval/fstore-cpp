#pragma once

#include "Database.test.h"

class BucketTest : public DatabaseTest {
protected:
    virtual auto tables() -> std::vector<std::string> override;
};
