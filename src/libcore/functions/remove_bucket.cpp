#include "object_store.h"

namespace fstore::core {
    auto object_store::remove_bucket(const UUID::uuid& bucket_id) -> void {
        database->remove_bucket(bucket_id);
    }
}
