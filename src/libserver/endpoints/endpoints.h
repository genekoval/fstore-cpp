#pragma once

#include <fstore/server/server.h>

namespace fstore::server::endpoint {
    auto add_object(protocol& proto) -> void;

    auto create_bucket(protocol& proto) -> void;

    auto fetch_bucket(protocol& proto) -> void;

    auto remove_bucket(protocol& proto) -> void;

    auto remove_object(protocol& proto) -> void;

    auto rename_bucket(protocol& proto) -> void;
}
