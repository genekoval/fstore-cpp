#include <internal/core/object_store.hpp>
#include <internal/server/router.hpp>

namespace fstore::server {
    router_context::router_context(
        core::object_store& store,
        const server_info& info
    ) :
        info(&info),
        store(&store)
    {}

    auto router_context::add_object(
        UUID::uuid bucket_id,
        std::optional<UUID::uuid> request,
        zipline::stream<net::socket> stream
    ) -> ext::task<object> {
        auto part_id = UUID::uuid();

        {
            auto part = store->get_part(request);
            part_id = part.id;
            co_await stream.read([&part](auto chunk) -> ext::task<void> {
                part.write(chunk);
                co_return;
            });
        }

        co_return co_await store->commit_part(bucket_id, part_id);
    }

    auto router_context::fetch_bucket(
        std::string bucket_name
    ) -> ext::task<bucket> {
        co_return co_await store->fetch_bucket(bucket_name);
    }

    auto router_context::get_object(
        UUID::uuid bucket_id,
        UUID::uuid object_id
    ) -> ext::task<file> {
        co_return co_await store->get_object(bucket_id, object_id);
    }

    auto router_context::get_object_metadata(
        UUID::uuid bucket_id,
        UUID::uuid object_id
    ) -> ext::task<object> {
        co_return co_await store->get_object_metadata(bucket_id, object_id);
    }

    auto router_context::get_server_info() -> ext::task<server_info> {
        co_return *info;
    }

    auto router_context::remove_object(
        UUID::uuid bucket_id,
        UUID::uuid object_id
    ) -> ext::task<object> {
        co_return co_await store->remove_object(bucket_id, object_id);
    }

    auto router_context::remove_objects(
        UUID::uuid bucket_id,
        std::vector<UUID::uuid> objects
    ) -> ext::task<remove_result> {
        co_return co_await store->remove_objects(bucket_id, objects);
    }
}
