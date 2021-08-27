#pragma once

#include <fstore/core/object_store.h>

#include <string_view>

namespace fstore::server {
    auto listen(
        core::object_store& store,
        const netcore::unix_socket& unix_socket,
        const std::function<void()>& callback
    ) -> void;
}
