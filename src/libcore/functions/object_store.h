#pragma once

#include <fstore/core/db/database.h>
#include <fstore/core/filesystem.h>
#include <fstore/core/object_store.h>

#include <timber/timber>
#include <uuid++/uuid.h>

namespace fstore::core {
    auto process_bucket_name(std::string_view name) -> std::string;
}
