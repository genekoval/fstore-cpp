#include "object_store.h"

namespace fstore::core {
    auto object_store::get_object_metadata(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> std::optional<object> {
        return database->get_object(bucket_id, object_id);
    }
}
