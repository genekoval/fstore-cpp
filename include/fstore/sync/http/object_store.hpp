#pragma once

#include <fstore/detail/http/object_store.hpp>
#include <fstore/model/blob.hpp>
#include <fstore/model/bucket.hpp>
#include <fstore/model/object.hpp>
#include <fstore/model/remove_result.hpp>
#include <fstore/model/server_info.hpp>

namespace fstore::sync::http {
    class object_store final {
        const detail::http::object_store client;
    public:
        object_store(std::string_view base_url);

        auto add_object(
            const UUID::uuid& bucket_id,
            const std::filesystem::path& path
        ) -> object;

        auto download_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            const std::filesystem::path& location
        ) -> void;

        auto fetch_bucket(std::string_view name) -> bucket;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> std::string;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            FILE* file
        ) -> void;

        auto get_object_metadata(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> object;

        auto get_server_info() -> server_info;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> object;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            std::span<const UUID::uuid> objects
        ) -> remove_result;
    };
}
