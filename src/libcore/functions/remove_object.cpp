#include "object_store.h"

namespace fstore::core {
    auto object_store::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> object {
        return database->remove_object(bucket_id, object_id);
    }
}
