#include <fstore/sync/http/bucket.hpp>

namespace fstore::sync::http {
    bucket::bucket(std::string_view base_url, const UUID::uuid& id) :
        id(id),
        store(base_url)
    {}

    auto bucket::download_object(
        const UUID::uuid& object_id,
        const std::filesystem::path& location
    ) -> void {
    }

    auto bucket::get_object(
        const UUID::uuid& object_id
    ) -> std::string {
        return store.get_object(id, object_id);
    }

    auto bucket::get_object(
        const UUID::uuid& object_id,
        FILE* file
    ) -> void {
        store.get_object(id, object_id, file);
    }

    auto bucket::get_object_metadata(
        const UUID::uuid& object_id
    ) -> object {
        return store.get_object_metadata(id, object_id);
    }

    auto bucket::remove_object(
        const UUID::uuid& object_id
    ) -> object {
        return store.remove_object(id, object_id);
    }

    auto bucket::remove_objects(
        std::span<const UUID::uuid> objects
    ) -> remove_result {
        return store.remove_objects(id, objects);
    }
}
