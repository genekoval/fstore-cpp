#pragma once

#include <fstore/core/object_store.h>
#include <fstore/server/server_info.h>

#include <string_view>

namespace fstore::server {
    auto create(
        core::object_store& store,
        const server_info& info
    ) -> netcore::server;
}
