#pragma once

#include <fstore/server/server.h>

namespace fstore::server {
    auto message(protocol& proto) -> void;
    auto print(protocol& proto) -> void;
    auto get_store_totals(protocol& proto) -> void;
}
