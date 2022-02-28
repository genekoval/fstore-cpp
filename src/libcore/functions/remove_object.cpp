#include "object_store.h"

namespace fstore::core {
    auto object_store::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> object {
        return database->remove_object(bucket_id, object_id);
    }
}
