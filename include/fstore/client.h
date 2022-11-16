#pragma once

#include <fstore/core/model.h>
#include <fstore/net/zipline/coder.h>
#include <fstore/net/zipline/protocol.h>
#include <fstore/server/server_info.h>

#include <ext/async_pool>

namespace fstore {
    using object_meta = core::object;
    using remove_result = core::remove_result;

    enum class event : net::event_t {
        add_object,
        fetch_bucket,
        get_object,
        get_object_metadata,
        get_server_info,
        remove_object,
        remove_objects
    };

    using client_type = zipline::client<net::socket, event, net::error_list>;

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
        std::unique_ptr<net::socket> socket;
        client_type client;
    public:
        object_store() = default;

        object_store(netcore::socket&& socket, const net::error_list& errors);

        auto add_object(
            const UUID::uuid& bucket_id,
            std::optional<UUID::uuid> part_id,
            std::size_t stream_size,
            const add_object_fn auto& pipe
        ) -> ext::task<object_meta> {
            co_await client.start(
                event::add_object,
                bucket_id,
                part_id,
                stream_size
            );

            auto part = fstore::part(client);
            co_await pipe(part);

            co_return co_await client.response<object_meta>();
        }

        auto deregister() -> void;

        auto fetch_bucket(std::string_view name) -> ext::task<core::bucket>;

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
        ) -> ext::task<object_meta>;

        auto get_server_info() -> ext::task<server::server_info>;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object_meta>;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            std::span<const UUID::uuid> objects
        ) -> ext::task<remove_result>;

        auto register_scoped() -> netcore::register_guard;
    };

    class bucket {
        object_store* store = nullptr;
    public:
        const UUID::uuid id;

        bucket() = default;

        bucket(object_store& store, const UUID::uuid& id);

        auto add(
            std::optional<UUID::uuid> part_id,
            std::size_t stream_size,
            const add_object_fn auto& pipe
        ) -> ext::task<object_meta> {
            co_return co_await store->add_object(
                id,
                part_id,
                stream_size,
                pipe
            );
        }

        auto get(const UUID::uuid& object_id) -> ext::task<blob>;

        auto get(const UUID::uuid& object_id, std::byte* buffer) -> ext::task<>;

        auto get(const UUID::uuid& object_id, std::ostream& out) -> ext::task<>;

        auto meta(const UUID::uuid& object_id) -> ext::task<object_meta>;

        auto remove(const UUID::uuid& object_id) -> ext::task<object_meta>;

        auto remove(std::span<const UUID::uuid> objects) -> ext::task<remove_result>;
    };

    class client {
    public:
        class provider {
            const std::string path;
            const net::error_list errors;
        public:
            provider() = default;

            provider(std::string_view path);

            auto provide() -> ext::task<object_store>;
        };

        using connection_pool = ext::async_pool<object_store, provider>;
        using connection = connection_pool::item;
    private:
        connection_pool pool;
    public:
        client() = default;

        client(std::string_view path);

        auto connect() -> ext::task<connection>;
    };
}
