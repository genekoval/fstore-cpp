#include <fstore/client.h>
#include <fstore/net/zipline/transfer.h>

#include <netcore/netcore>

namespace fstore {
    object_store::object_store(std::string_view path) :
        endpoint(path)
    {}

    auto object_store::add_object(
        std::string_view bucket_id,
        std::optional<std::string_view> part_id,
        std::size_t stream_size,
        std::function<void(part&&)> pipe
    ) -> object_meta {
        auto client = connect();

        client.start(
            event::add_object,
            bucket_id,
            part_id,
            stream_size
        );

        pipe(part(client));

        return client.response<object_meta>();
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
        return client(errors, net::socket(netcore::connect(endpoint)));
    }

    auto object_store::fetch_bucket(std::string_view name) -> service::bucket {
        return connect().send<service::bucket>(
            event::fetch_bucket,
            name
        );
    }

    auto object_store::get_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> blob {
        return connect().send<blob>(
            event::get_object,
            bucket_id,
            object_id
        );
    }

    auto object_store::get_object(
        std::string_view bucket_id,
        std::string_view object_id,
        std::byte* buffer
    ) -> void {
        auto client = connect();
        auto stream = client.send<net::data_stream>(
            event::get_object,
            bucket_id,
            object_id
        );

        auto index = 0;

        stream.read([&index, &buffer](auto&& chunk) {
            std::memcpy(&buffer[index], chunk.data(), chunk.size());
            index += chunk.size();
        });
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
