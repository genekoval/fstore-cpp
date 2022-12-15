#include "object_store.h"

#include <fstore/error.h>

#include <ext/string.h>

namespace fstore::core {
    auto process_bucket_name(std::string_view name) -> std::string {
        const auto trimmed = ext::trim(std::string(name));
        if (trimmed.empty()) throw invalid_data("bucket name cannot be empty");

        return trimmed;
    }
}
