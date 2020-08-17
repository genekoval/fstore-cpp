#include <fstore/client.h>
#include <fstore/server/transfer.h>

#include <netcore/client.h>

namespace fstore {
    object_store::object_store(std::string_view path) :
        endpoint(path)
    {}

    auto object_store::add_object(
        std::string_view bucket_id,
        std::string_view path
    ) -> model::object {
        return connect().send<model::object>(
            event::add_object,
            bucket_id,
            path
        );
    }

    auto object_store::connect() -> client {
        socket = netcore::connect(endpoint);
        return client(socket);
    }

    auto object_store::fetch_bucket(std::string_view name) -> model::bucket {
        return connect().send<model::bucket>(
            event::fetch_bucket,
            name
        );
    }

    auto object_store::get_object_metadata(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> std::optional<model::object> {
        return connect().send<std::optional<model::object>>(
            event::get_object_metadata,
            bucket_id,
            object_id
        );
    }

    auto object_store::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> model::object {
        return connect().send<model::object>(
            event::remove_object,
            bucket_id,
            object_id
        );
    }
}
