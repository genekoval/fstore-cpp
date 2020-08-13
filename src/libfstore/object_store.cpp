#include <fstore/client.h>
#include <fstore/server/transfer.h>

#include <netcore/client.h>

namespace fstore {
    object_store::object_store(std::string_view path) :
        sock(netcore::connect(path)),
        client(sock)
    {}

    auto object_store::add_object(
        std::string_view bucket_id,
        std::string_view path
    ) -> model::object {
        return client.send<model::object>(
            event::add_object,
            bucket_id,
            path
        );
    }

    auto object_store::fetch_bucket(std::string_view name) -> model::bucket {
        return client.send<model::bucket>(
            event::fetch_bucket,
            name
        );
    }

    auto object_store::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> model::object {
        return client.send<model::object>(
            event::remove_object,
            bucket_id,
            object_id
        );
    }
}
