#pragma once

#include <fstore/core/db/model.h>

#include <netcore/netcore>

namespace fstore::core {
    using bucket = db::bucket;
    using object = db::object;
    using remove_result = db::remove_result;
    using store_totals = db::store_totals;

    struct file {
        netcore::fd fd;
        std::size_t size;
    };
}
