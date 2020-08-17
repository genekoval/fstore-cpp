#pragma once

#include <fstore/models.h>
#include <fstore/server/server.h>

namespace fstore {
    class object_store {
        enum class event : server::event_t {
            add_object,
            fetch_bucket,
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

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> model::object;
    };

    class bucket {
        const std::string id;
        object_store* store;
    public:
        bucket(std::string_view id, object_store& store);

        auto add(std::string_view path) -> model::object;

        auto remove(std::string_view object_id) -> model::object;
    };
}
