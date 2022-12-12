#include <fstore/client.h>

namespace fstore {
    object_store::object_store(netcore::socket&& socket) :
        client(new client_type(
            net::error_list::thrower(),
            net::socket(std::forward<netcore::socket>(socket))
        ))
    {}

    auto object_store::deregister() -> void {
        client->inner.deregister();
    }

    auto object_store::fetch_bucket(
        std::string_view name
    ) -> ext::task<core::bucket> {
        co_return co_await client->send<core::bucket>(
            event::fetch_bucket,
            name
        );
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<blob> {
        co_return co_await client->send<blob>(
            event::get_object,
            bucket_id,
            object_id
        );
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id,
        std::byte* buffer
    ) -> ext::task<> {
        auto stream = co_await client->send<zipline::stream<client_type>>(
            event::get_object,
            bucket_id,
            object_id
        );

        auto index = 0;

        co_await stream.read([&index, &buffer](auto&& chunk) -> ext::task<> {
            std::memcpy(&buffer[index], chunk.data(), chunk.size());
            index += chunk.size();
            co_return;
        });
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id,
        std::ostream& out
    ) -> ext::task<> {
        auto stream = co_await client->send<zipline::stream<client_type>>(
            event::get_object,
            bucket_id,
            object_id
        );

        co_await stream.read([&out](auto&& chunk) -> ext::task<> {
            out.write(
                reinterpret_cast<const char*>(chunk.data()),
                chunk.size()
            );

            co_return;
        });
    }

    auto object_store::get_object_metadata(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object_meta> {
        co_return co_await client->send<object_meta>(
            event::get_object_metadata,
            bucket_id,
            object_id
        );
    }

    auto object_store::get_server_info() -> ext::task<server::server_info> {
        co_return co_await client->send<server::server_info>(
            event::get_server_info
        );
    }

    auto object_store::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object_meta> {
        co_return co_await client->send<object_meta>(
            event::remove_object,
            bucket_id,
            object_id
        );
    }

    auto object_store::remove_objects(
        const UUID::uuid& bucket_id,
        std::span<const UUID::uuid> objects
    ) -> ext::task<core::remove_result> {
        co_return co_await client->send<remove_result>(
            event::remove_objects,
            bucket_id,
            objects
        );
    }

    auto object_store::register_scoped() -> netcore::register_guard {
        return client->inner.register_scoped();
    }
}
