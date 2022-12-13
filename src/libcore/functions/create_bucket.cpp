#include "object_store.h"

namespace fstore::core {
    auto object_store::create_bucket(std::string_view name) -> bucket {
        const auto processed_name = process_bucket_name(name);

        return database->create_bucket(processed_name);
    }
}
