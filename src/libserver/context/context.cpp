#include "context.h"

#include <fstore/error.h>
#include <fstore/net/zipline/protocol.h>

#include <span>

namespace fstore::server {
    context::context(core::object_store& store, const server_info& info) :
        info(&info),
        store(&store)
    {}

    auto context::add_object(
        UUID::uuid bucket_id,
        std::optional<UUID::uuid> request,
        net::data_stream stream
    ) -> ext::task<core::object> {
        auto part_id = UUID::uuid();

        {
            auto part = store->get_part(request);
            part_id = part.id;
            co_await stream.read([&part](auto chunk) -> ext::task<void> {
                part.write(chunk);
                co_return;
            });
        }

        co_return store->commit_part(bucket_id, part_id);
    }

    auto context::fetch_bucket(
        std::string bucket_name
    ) -> ext::task<core::bucket> {
        co_return store->fetch_bucket(bucket_name);
    }

    auto context::get_object(
        UUID::uuid bucket_id,
        UUID::uuid object_id
    ) -> ext::task<core::file> {
        auto file = store->get_object(bucket_id, object_id);
        if (!file) throw fstore_error("bucket does not contain object");

        co_return std::move(*file);
    }

    auto context::get_object_metadata(
        UUID::uuid bucket_id,
        UUID::uuid object_id
    ) -> ext::task<core::object> {
        auto object = store->get_object_metadata(bucket_id, object_id);
        if (!object) throw fstore_error("bucket does not contain object");

        co_return std::move(*object);
    }

    auto context::get_server_info() -> ext::task<server_info> {
        co_return *info;
    }

    auto context::remove_object(
        UUID::uuid bucket_id,
        UUID::uuid object_id
    ) -> ext::task<core::object> {
        co_return store->remove_object(bucket_id, object_id);
    }

    auto context::remove_objects(
        UUID::uuid bucket_id,
        std::vector<UUID::uuid> objects
    ) -> ext::task<core::remove_result> {
        co_return store->remove_objects(bucket_id, objects);
    }
}
