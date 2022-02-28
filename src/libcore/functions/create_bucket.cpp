#include "object_store.h"

namespace fstore::core {
    auto object_store::create_bucket(std::string_view name) -> bucket {
        const auto uuid = generate_uuid();
        const auto processed_name = process_bucket_name(name);

        return database->create_bucket(uuid.string(), processed_name);
    }
}
