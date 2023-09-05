#pragma once

#include <http/http>

namespace fstore::detail::http {
    using request = ::http::client::request;

    class object_store {
        const ::http::client client;
    public:
        object_store(std::string_view base_url);

        object_store(std::string_view base_url, ::http::session& session);

        auto add_object(
            const UUID::uuid& bucket_id,
            const std::filesystem::path& path
        ) const -> request;

        auto fetch_bucket(std::string_view name) const -> request;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) const -> request;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::byte* buffer
        ) const -> request;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::ostream& out
        ) const -> request;

        auto get_object_metadata(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) const -> request;

        auto get_server_info() const -> request;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) const -> request;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            std::span<const UUID::uuid> objects
        ) const -> request;
    };
}
