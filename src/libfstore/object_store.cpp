#include <fstore/except.hpp>
#include <fstore/object_store.hpp>

namespace fstore {
    object_store::object_store(net::socket&& socket) :
        client(new client_type(
            error_list::thrower(),
            std::forward<net::socket>(socket)
        )) {}

    auto object_store::connected() -> bool { return client->inner.connected(); }

    auto object_store::failed() const noexcept -> bool {
        return client->inner.failed();
    }

    auto object_store::fetch_bucket(std::string_view name)
        -> ext::task<bucket> {
        co_return co_await client->send<bucket>(event::fetch_bucket, name);
    }

    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<blob> {
        co_return co_await client
            ->send<blob>(event::get_object, bucket_id, object_id);
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
    ) -> ext::task<object> {
        co_return co_await client
            ->send<object>(event::get_object_metadata, bucket_id, object_id);
    }

    auto object_store::get_server_info() -> ext::task<server_info> {
        co_return co_await client->send<server_info>(event::get_server_info);
    }

    auto object_store::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        co_return co_await client
            ->send<object>(event::remove_object, bucket_id, object_id);
    }

    auto object_store::remove_objects(
        const UUID::uuid& bucket_id,
        std::span<const UUID::uuid> objects
    ) -> ext::task<remove_result> {
        co_return co_await client
            ->send<remove_result>(event::remove_objects, bucket_id, objects);
    }
}
