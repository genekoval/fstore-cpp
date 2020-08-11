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

        auto create_bucket(std::string_view name) -> model::bucket;

        auto fetch_bucket(std::string_view name) -> model::bucket;

        auto remove_bucket(std::string_view bucket_id) -> void;

        auto remove_object(
            std::string_view bucket_id,
            std::string_view object_id
        ) -> model::object;

        auto rename_bucket(
            std::string_view bucket_id,
            std::string_view bucket_name
        ) -> void;
    };
}
