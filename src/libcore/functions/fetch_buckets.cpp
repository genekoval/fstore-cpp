#include "object_store.h"

namespace fstore::core {
    auto object_store::fetch_buckets() -> std::vector<bucket> {
        return database->fetch_buckets();
    }

    auto object_store::fetch_buckets(
        const std::vector<std::string>& names
    ) -> std::vector<bucket> {
        return database->fetch_buckets(names);
    }
}
