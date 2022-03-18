#pragma once

#include "Database.test.h"

class ObjectTest : public DatabaseTest {
protected:
    static constexpr auto bucket_id = "69595d19-8381-4c1f-ac52-52680df83675";
    static constexpr auto bucket_name = "object-test";

    static auto SetUpTestSuite() -> void;

    static auto TearDownTestSuite() -> void;

    const std::vector<fstore::core::db::object> objects = {
        {
            .id = "a4f33eab-1fbf-49ab-af6b-9fc72714f8c5",
            .hash =
                "eab32d918fc1c07d87eddb59a4508666"
                "6f9117538d6d9c40ee0efeda635bd330",
            .size = 7,
            .type = "text",
            .subtype = "plain"
        },
        {
            .id = "4ca61fdd-9058-4d08-a16b-744d3afc4b6d",
            .hash = "1c661c11465816c9bbcd70ac08a655cd"
                    "bf8dedf2122bd0ed1cb2d6b36b0e09ed",
            .size = 250,
            .type = "text",
            .subtype = "plain"
        },
        {
            .id = "d7303d5a-3892-4888-a47e-b201f5219ee1",
            .hash = "ca3a6440be51f6a8687d25a683ff85c0"
                    "a9676d45b2f57f27b841b6b259893dc9",
            .size = 500,
            .type = "text",
            .subtype = "plain"
        }
    };

    auto add_object(const fstore::core::db::object& object) -> void;

    auto bucket() -> fstore::core::db::bucket;

    auto bucket_size() -> std::size_t;

    virtual auto tables() -> std::vector<std::string> override;
};
