#pragma once

#include <fstore/server/server.h>

namespace fstore::server::endpoint {
    auto add_object(protocol&) -> void;

    auto create_object_from_file(protocol&) -> void;

    auto fetch_bucket(protocol&) -> void;

    auto get_object(protocol&) -> void;

    auto get_object_metadata(protocol&) -> void;

    auto remove_object(protocol&) -> void;
}
