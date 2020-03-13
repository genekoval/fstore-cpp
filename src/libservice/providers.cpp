#include <service.h>

namespace fstore::service {
    auto local_store() -> std::unique_ptr<core::object_store> {
        return std::make_unique<object_store>();
    }
}
