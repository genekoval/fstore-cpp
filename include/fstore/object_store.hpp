#pragma once

#include "event.hpp"
#include "model/blob.hpp"
#include "model/bucket.hpp"
#include "model/object.hpp"
#include "model/remove_result.hpp"
#include "model/server_info.hpp"
#include "net/zipline/socket.hpp"

namespace fstore {
    using client_type = zipline::client<net::socket, event>;

    class part {
        client_type& out;
    public:
        part(client_type& out);

        auto write(std::span<const std::byte> data) -> ext::task<>;
    };

    template <typename F>
    concept add_object_fn =
        requires(const F& f, part& p) {
            { f(p) } -> std::same_as<ext::task<>>;
        };

    class object_store {
        std::unique_ptr<client_type> client;
    public:
        object_store() = default;

        object_store(netcore::socket&& socket);

        auto add_object(
            const UUID::uuid& bucket_id,
            std::optional<UUID::uuid> part_id,
            std::size_t stream_size,
            const add_object_fn auto& pipe
        ) -> ext::task<object> {
            co_await client->start(
                event::add_object,
                bucket_id,
                part_id,
                stream_size
            );

            auto part = fstore::part(*client);
            co_await pipe(part);

            co_return co_await client->read_response<object>();
        }

        auto fetch_bucket(std::string_view name) -> ext::task<bucket>;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<blob>;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::byte* buffer
        ) -> ext::task<>;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::ostream& out
        ) -> ext::task<>;

        auto get_object_metadata(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        auto get_server_info() -> ext::task<server_info>;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            std::span<const UUID::uuid> objects
        ) -> ext::task<remove_result>;
    };

    using client = netcore::client<object_store>;
}
