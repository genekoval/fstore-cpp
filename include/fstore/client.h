#pragma once

#include <fstore/models.h>
#include <fstore/net/zipline/protocol.h>

#include <ext/dynarray.h>

namespace fstore {
    using object_meta = model::object;
    using blob = ext::dynarray<std::byte>;

    enum class event : net::event_t {
        add_object,
        create_object_from_file,
        fetch_bucket,
        get_object,
        get_object_metadata,
        remove_object
    };

    using client = zipline::client<net::socket, event, net::error_list>;

    class part {
        client* out;
    public:
        part(client& out);

        auto write(std::span<const std::byte> data) -> void;
    };

    class object_store {
        const std::string endpoint;
        const net::error_list errors;

        auto connect() -> client;
    public:
        object_store(std::string_view path);

        auto add_object(
            std::string_view bucket_id,
            std::optional<std::string_view> part_id,
            std::size_t stream_size,
            std::function<void(part&&)> pipe
        ) -> object_meta;

        auto create_object_from_file(
            std::string_view bucket_id,
            std::string_view path
        ) -> object_meta;

        auto fetch_bucket(std::string_view name) -> model::bucket;

        auto get_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> blob;

        auto get_object(
            std::string_view bucket_id,
            std::string_view object_id,
            std::byte* buffer
        ) -> void;

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

        auto add(
            std::optional<std::string_view> part_id,
            std::size_t stream_size,
            std::function<void(part&&)> pipe
        ) -> object_meta;

        auto add(std::string_view path) -> object_meta;

        auto get(
            std::string_view object_id
        ) -> blob;

        auto get(
            std::string_view object_id,
            std::byte* buffer
        ) -> void;

        auto meta(
            std::string_view object_id
        ) -> object_meta;

        auto remove(std::string_view object_id) -> object_meta;
    };
}
