#pragma once

#include <entix/entity>
#include <string>

namespace fstore::repo {
    struct bucket : entix::entity<5> {
        std::string id;
        std::string name;
        std::string date_created;
        std::size_t size;
        uintmax_t space_used;
    };

    struct object : entix::entity<5> {
        std::string id;
        std::string hash;
        uintmax_t size;
        std::string mime_type;
        std::string date_added;
    };

    struct remove_result : entix::entity<2> {
        std::size_t objects_removed;
        uintmax_t space_freed;
    };

    struct store_totals : entix::entity<3> {
        std::size_t buckets;
        std::size_t objects;
        uintmax_t space_used;
    };
}
