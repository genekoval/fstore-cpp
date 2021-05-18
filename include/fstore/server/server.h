#pragma once

#include <fstore/service/object_store.h>

#include <string_view>

namespace fstore::server {
    auto listen(
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void;
}
