#include <fstore/client.h>

namespace fstore {
    bucket::bucket(object_store& store, std::string_view id) :
        store(&store),
        id(id)
    {}

    auto bucket::add(
        std::optional<std::string_view> part_id,
        std::size_t stream_size,
        std::function<void(part&&)> pipe
    ) -> object_meta {
        return store->add_object(id, part_id, stream_size, pipe);
    }

    auto bucket::add(std::string_view path) -> object_meta {
        return store->create_object_from_file(id, path);
    }

    auto bucket::get(
        std::string_view object_id
    ) -> blob {
        return store->get_object(id, object_id);
    }

    auto bucket::get(
        std::string_view object_id,
        std::byte* buffer
    ) -> void {
        store->get_object(id, object_id, buffer);
    }

    auto bucket::get(
        std::string_view object_id,
        std::ostream& out
    ) -> void {
        store->get_object(id, object_id, out);
    }

    auto bucket::meta(
        std::string_view object_id
    ) -> object_meta {
        return store->get_object_metadata(id, object_id);
    }

    auto bucket::remove(std::string_view object_id) -> object_meta {
        return store->remove_object(id, object_id);
    }

    auto bucket::remove(
        std::span<const std::string> objects
    ) -> remove_result {
        return store->remove_objects(id, objects);
    }
}
