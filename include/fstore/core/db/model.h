#pragma once

#include <chrono>
#include <string>
#include <uuid++/uuid++>

namespace fstore::core::db {
    using time_point = std::chrono::time_point<
        std::chrono::system_clock,
        std::chrono::milliseconds
    >;

    struct bucket {
        UUID::uuid id;
        std::string name;
        time_point date_created;
        std::size_t size;
        uintmax_t space_used;

        auto operator<=>(const bucket&) const = default;
    };

    struct object_error {
        UUID::uuid id;
        std::string message;

        auto operator<=>(const object_error&) const = default;
    };

    struct object {
        UUID::uuid id;
        std::string hash;
        uintmax_t size;
        std::string type;
        std::string subtype;
        time_point date_added;

        auto operator<=>(const object&) const = default;

        auto mime_type() const -> std::string {
            auto result = std::string();
            result.reserve(type.size() + subtype.size() + 1);

            result += type;
            result += "/";
            result += subtype;

            return result;
        }
    };

    struct remove_result {
        std::size_t objects_removed;
        uintmax_t space_freed;

        auto operator<=>(const remove_result&) const = default;
    };

    struct store_totals {
        std::size_t buckets;
        std::size_t objects;
        uintmax_t space_used;

        auto operator<=>(const store_totals&) const = default;
    };
}
