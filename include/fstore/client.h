#pragma once

#include <fstore/models.h>
#include <fstore/server/server.h>

namespace fstore {
    class object_store {
        enum class event : server::event_t {
            add_object,
            fetch_bucket,
            get_object_metadata,
            remove_object,
        };

        using protocol = zipline::protocol<netcore::socket>;
        using client = zipline::client<protocol, event>;

        const std::string endpoint;
        netcore::socket socket;

        auto connect() -> client;
    public:
        object_store(std::string_view path);

        auto add_object(
            std::string_view bucket_id,
            std::string_view path
        ) -> model::object;

        auto fetch_bucket(std::string_view name) -> model::bucket;

        auto get_object_metadata(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> std::optional<model::object>;

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> model::object;
    };

    class object {
    public:
        const model::object metadata;
    };

    class bucket {
        const std::string id;
        object_store* store;
    public:
        bucket(std::string_view id, object_store& store);

        auto operator[](std::string_view object_id) -> std::optional<object>;

        auto add(std::string_view path) -> model::object;

        auto remove(std::string_view object_id) -> model::object;
    };
}
