#pragma once

#include <fstore/core/db/model.h>

#include <atomic>
#include <netcore/netcore>

namespace fstore::core {
    using bucket = db::bucket;
    using object = db::object;
    using object_error = db::object_error;
    using remove_result = db::remove_result;
    using store_totals = db::store_totals;

    struct check_progress {
        std::atomic_ulong completed;
        std::size_t total;
    };

    struct file {
        netcore::fd fd;
        std::size_t size;
    };
}
