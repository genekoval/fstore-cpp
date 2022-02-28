#include "object_store.h"

namespace fstore::core {
    auto object_store::fetch_bucket(std::string_view name) -> bucket {
        return database->fetch_bucket(name);
    }
}
