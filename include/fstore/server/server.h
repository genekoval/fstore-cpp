#pragma once

#include <fstore/service/object_store.h>

#include <string_view>

namespace fstore::server {
    auto listen(
        service::object_store& store,
        const netcore::unix_socket& unix_socket,
        const std::function<void()>& callback
    ) -> void;
}
