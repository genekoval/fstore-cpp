#pragma once

#include <fstore/repo/model.h>

#include <netcore/netcore>

namespace fstore::core {
    using bucket = repo::bucket;
    using object = repo::object;
    using store_totals = repo::store_totals;

    struct file {
        netcore::fd fd;
        std::size_t size;
    };
}
