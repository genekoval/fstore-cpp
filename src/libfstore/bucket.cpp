#include <fstore/client.h>

namespace fstore {
    bucket::bucket(std::string_view id, object_store& store) :
        id(id),
        store(&store)
    {}

    auto bucket::add(std::string_view path) -> object_meta {
        return store->add_object(id, path);
    }

    auto bucket::get(
        std::string_view object_id
    ) -> object_content {
        return store->get_object(id, object_id);
    }

    auto bucket::meta(
        std::string_view object_id
    ) -> object_meta {
        return store->get_object_metadata(id, object_id);
    }

    auto bucket::remove(std::string_view object_id) -> object_meta {
        return store->remove_object(id, object_id);
    }
}
