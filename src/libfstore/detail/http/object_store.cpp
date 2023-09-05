#include <fstore/detail/http/object_store.hpp>

namespace fstore::detail::http {
    object_store::object_store(std::string_view base_url) : client(base_url) {}

    object_store::object_store(
        std::string_view base_url,
        ::http::session& session
    ) :
        client(base_url, session)
    {}

    auto object_store::add_object(
        const UUID::uuid& bucket_id,
        const std::filesystem::path& path
    ) const -> request {
        auto request = client.post("object", bucket_id);

        request.upload(path);

        return request;
    }

    auto object_store::fetch_bucket(std::string_view name) const -> request {
        return client.get("bucket", name);
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) const -> request {
        return client.get("data", bucket_id, object_id);
    }

    auto object_store::get_object_metadata(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) const -> request {
        return client.get("object", bucket_id, object_id);
    }

    auto object_store::get_server_info() const -> request {
        return client.get();
    }

    auto object_store::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) const -> request {
        return client.del("object", bucket_id, object_id);
    }

    auto object_store::remove_objects(
        const UUID::uuid& bucket_id,
        std::span<const UUID::uuid> objects
    ) const -> request {
        auto request = client.del("object", bucket_id);

        request.text(ext::join(objects, "\n"));

        return request;
    }
}
