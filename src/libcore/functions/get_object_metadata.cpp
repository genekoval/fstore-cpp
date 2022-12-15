#include "object_store.h"

namespace fstore::core {
    auto object_store::get_object_metadata(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> object {
        return database->get_object(bucket_id, object_id);
    }
}
