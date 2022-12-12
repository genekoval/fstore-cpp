#pragma once

#include <fstore/net/zipline/protocol.h>
#include <fstore/core/object_store.h>
#include <fstore/server/server_info.h>

namespace fstore::server {
    class router_context {
        const server_info* const info;
        core::object_store* store;
    public:
        router_context(core::object_store& store, const server_info& info);

        auto add_object(
            UUID::uuid bucket_id,
            std::optional<UUID::uuid> request,
            zipline::stream<net::socket> stream
        ) -> ext::task<core::object>;

        auto fetch_bucket(std::string bucket_name) -> ext::task<core::bucket>;

        auto get_object(
            UUID::uuid bucket_id,
            UUID::uuid object_id
        ) -> ext::task<core::file>;

        auto get_object_metadata(
            UUID::uuid bucket_id,
            UUID::uuid object_id
        ) -> ext::task<core::object>;

        auto get_server_info() -> ext::task<server_info>;

        auto remove_object(
            UUID::uuid bucket_id,
            UUID::uuid object_id
        ) -> ext::task<core::object>;

        auto remove_objects(
            UUID::uuid bucket_id,
            std::vector<UUID::uuid> objects
        ) -> ext::task<core::remove_result>;
    };
}
