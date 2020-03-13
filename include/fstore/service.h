#pragma once

#include <fstore/core.h>

namespace fstore::service {
    /**
     * Returns an Object Store implementation that allows for manipulating
     * data directly.
     *
     * returns: An Object Store instance for manipulating local data.
     */
    auto local_store() -> std::unique_ptr<core::object_store>;
}
