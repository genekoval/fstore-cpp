#pragma once

#include <fstore/models.h>
#include <fstore/server/server.h>

#include <span>

namespace fstore {
    using byte_vector = std::vector<std::byte>;
    using object_meta = model::object;

    struct object_content {
        const std::string mime_type;
        const byte_vector data;
    };

    enum class event : server::event_t {
        add_object,
        create_object_from_file,
        fetch_bucket,
        get_object,
        get_object_metadata,
        remove_object,
    };

    class object_store {
        using protocol = zipline::protocol<netcore::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;

        auto connect() -> client;
    public:
        object_store(std::string_view path);

        auto add_object(
            std::string_view bucket_id,
            std::span<const std::byte> data
        ) -> object_meta;

        auto create_object_from_file(
            std::string_view bucket_id,
            std::string_view path
        ) -> object_meta;

        auto fetch_bucket(std::string_view name) -> model::bucket;

        auto get_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> object_content;

        auto get_object_metadata(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> object_meta;

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> object_meta;
    };

    class bucket {
        const std::string id;
        object_store* store;
    public:
        bucket(std::string_view id, object_store& store);

        auto add(const void* buffer, std::size_t len) -> object_meta;

        auto add(std::string_view path) -> object_meta;

        auto get(
            std::string_view object_id
        ) -> object_content;

        auto meta(
            std::string_view object_id
        ) -> object_meta;

        auto remove(std::string_view object_id) -> object_meta;
    };
}
