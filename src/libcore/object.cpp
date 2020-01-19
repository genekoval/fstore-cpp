#include <core.h>

namespace fstore::core {
    object_impl::object_impl(uuid id) : _id(id) {}

    uuid object_impl::id() const { return _id; }
}
