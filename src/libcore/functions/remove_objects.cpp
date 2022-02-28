#include "object_store.h"

namespace fstore::core {
    auto object_store::remove_objects(
        std::string_view bucket_id,
        const std::vector<std::string>& objects
    ) -> remove_result {
        return database->remove_objects(bucket_id, objects);
    }
}
