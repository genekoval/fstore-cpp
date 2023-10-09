#include <fstore/sync/http/object_store.hpp>

namespace fstore::sync::http {
    object_store::object_store(std::string_view base_url) : client(base_url) {}

    auto object_store::add_object(
        const UUID::uuid& bucket_id,
        const std::filesystem::path& path
    ) -> object {
        return client.add_object(bucket_id, path).send<object>();
    }

    auto object_store::download_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id,
        const std::filesystem::path& location
    ) -> void {
        client.get_object(bucket_id, object_id).download(location);
    }

    auto object_store::fetch_bucket(std::string_view name) -> bucket {
        return client.fetch_bucket(name).send<bucket>();
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> std::string {
        return client.get_object(bucket_id, object_id).data();
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id,
        FILE* file
    ) -> void {
        client.get_object(bucket_id, object_id).pipe(file);
    }

    auto object_store::get_object_metadata(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> object {
        return client.get_object_metadata(bucket_id, object_id).send<object>();
    }

    auto object_store::get_server_info() -> server_info {
        return client.get_server_info().send<server_info>();
    }

    auto object_store::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> object {
        return client.remove_object(bucket_id, object_id).send<object>();
    }

    auto object_store::remove_objects(
        const UUID::uuid& bucket_id,
        std::span<const UUID::uuid> objects
    ) -> remove_result {
        return client.remove_objects(bucket_id, objects).send<remove_result>();
    }
}
