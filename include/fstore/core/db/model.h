#pragma once

#include <entix/entity>
#include <string>
#include <uuid++/uuid++>

namespace fstore::core::db {
    struct bucket : entix::entity<5> {
        UUID::uuid id;
        std::string name;
        std::string date_created;
        std::size_t size;
        uintmax_t space_used;

        auto operator<=>(const bucket&) const = default;
    };

    struct object : entix::entity<6> {
        UUID::uuid id;
        std::string hash;
        uintmax_t size;
        std::string type;
        std::string subtype;
        std::string date_added;

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

    struct remove_result : entix::entity<2> {
        std::size_t objects_removed;
        uintmax_t space_freed;

        auto operator<=>(const remove_result&) const = default;
    };

    struct store_totals : entix::entity<3> {
        std::size_t buckets;
        std::size_t objects;
        uintmax_t space_used;

        auto operator<=>(const store_totals&) const = default;
    };
}
