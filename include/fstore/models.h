#pragma once

#include <netcore/netcore>
#include <string>

namespace fstore::model {
    struct bucket {
        std::string id;
        std::string name;
        std::string date_created;
        std::size_t size;
        uintmax_t space_used;
    };

    struct file {
        netcore::fd fd;
        std::size_t size;
    };

    struct object {
        std::string id;
        std::string hash;
        uintmax_t size;
        std::string mime_type;
        std::string date_added;
    };

    struct object_store {
        std::size_t buckets;
        std::size_t objects;
        uintmax_t space_used;
    };
}
