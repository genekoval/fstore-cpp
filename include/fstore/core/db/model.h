#pragma once

#include <entix/entity>
#include <string>

namespace fstore::core::db {
    struct bucket : entix::entity<5> {
        std::string id;
        std::string name;
        std::string date_created;
        std::size_t size;
        uintmax_t space_used;

        auto operator==(const bucket&) const -> bool = default;
    };

    struct object : entix::entity<5> {
        std::string id;
        std::string hash;
        uintmax_t size;
        std::string mime_type;
        std::string date_added;

        auto operator==(const object&) const -> bool = default;
    };

    struct remove_result : entix::entity<2> {
        std::size_t objects_removed;
        uintmax_t space_freed;

        auto operator==(const remove_result&) const -> bool = default;
    };

    struct store_totals : entix::entity<3> {
        std::size_t buckets;
        std::size_t objects;
        uintmax_t space_used;

        auto operator==(const store_totals&) const -> bool = default;
    };
}
