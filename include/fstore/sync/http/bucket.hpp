#pragma once

#include "object_store.hpp"

namespace fstore::sync::http {
    class bucket final {
        UUID::uuid id;
        object_store store;
    public:
        bucket(std::string_view base_url, const UUID::uuid& id);

        auto add_object(const std::filesystem::path& path) -> object;

        auto download_object(
            const UUID::uuid& object_id,
            const std::filesystem::path& location
        ) -> void;

        auto get_object(
            const UUID::uuid& object_id
        ) -> std::string;

        auto get_object(
            const UUID::uuid& object_id,
            FILE* file
        ) -> void;

        auto get_object_metadata(
            const UUID::uuid& object_id
        ) -> object;

        auto remove_object(
            const UUID::uuid& object_id
        ) -> object;

        auto remove_objects(
            std::span<const UUID::uuid> objects
        ) -> remove_result;
    };
}
