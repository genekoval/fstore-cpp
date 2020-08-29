#include <fstore/client.h>
#include <fstore/server/transfer.h>

#include <netcore/client.h>

namespace fstore {
    object_store::object_store(std::string_view path) :
        endpoint(path)
    {}

    auto object_store::add_object(
        std::string_view bucket_id,
            std::span<const std::byte> data
    ) -> object_meta {
        return connect().send<object_meta>(
            event::add_object,
            bucket_id,
            data
        );
    }

    auto object_store::create_object_from_file(
        std::string_view bucket_id,
        std::string_view path
    ) -> object_meta {
        return connect().send<object_meta>(
            event::create_object_from_file,
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

    auto object_store::get_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> object_content {
        return connect().send<object_content>(
            event::get_object,
            bucket_id,
            object_id
        );
    }

    auto object_store::get_object_metadata(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> object_meta {
        return connect().send<object_meta>(
            event::get_object_metadata,
            bucket_id,
            object_id
        );
    }

    auto object_store::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> object_meta {
        return connect().send<object_meta>(
            event::remove_object,
            bucket_id,
            object_id
        );
    }
}
