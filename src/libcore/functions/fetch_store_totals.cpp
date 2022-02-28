#include "object_store.h"

namespace fstore::core {
    auto object_store::fetch_store_totals() -> store_totals {
        return database->fetch_store_totals();
    }
}
