#include "object_store.h"

namespace fstore::core {
    auto object_store::get_errors() -> std::vector<object_error> {
        return database->get_errors();
    }
}
