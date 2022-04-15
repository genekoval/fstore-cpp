#include "object_store.h"

namespace fstore::core {
    auto object_store::rename_bucket(
        const UUID::uuid& bucket_id,
        std::string_view bucket_name
    ) -> void {
        const auto name = process_bucket_name(bucket_name);
        database->rename_bucket(bucket_id, name);
    }
}
