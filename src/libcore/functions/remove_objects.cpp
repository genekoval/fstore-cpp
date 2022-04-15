#include "object_store.h"

namespace fstore::core {
    auto object_store::remove_objects(
        const UUID::uuid& bucket_id,
        const std::vector<UUID::uuid>& objects
    ) -> remove_result {
        return database->remove_objects(bucket_id, objects);
    }
}
