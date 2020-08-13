#include <fstore/client.h>

namespace fstore {
    bucket::bucket(std::string_view id, object_store& store) :
        id(id),
        store(&store)
    {}

    auto bucket::add(std::string_view path) -> model::object {
        return store->add_object(id, path);
    }

    auto bucket::remove(std::string_view object_id) -> model::object {
        return store->remove_object(id, object_id);
    }
}
