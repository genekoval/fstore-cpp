#include "object_store.h"

namespace fstore::core {
    auto object_store::remove_bucket(std::string_view bucket_id) -> void {
        database->remove_bucket(bucket_id);
    }
}
