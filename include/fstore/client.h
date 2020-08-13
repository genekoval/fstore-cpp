#pragma once

#include <fstore/models.h>
#include <fstore/server/server.h>

namespace fstore {
    class object_store {
        using protocol = zipline::protocol<netcore::socket>;

        enum class event : server::event_t {
            add_object,
            create_bucket,
            fetch_bucket,
            remove_bucket,
            remove_object,
            rename_bucket
        };

        const protocol::socket_type sock;
        zipline::client<protocol, event> client;
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

        bucket(std::string_view id, object_store& store);
    public:
        auto add(std::string_view path) -> model::object;

        auto remove(std::string_view object_id) -> model::object;
    };
}
