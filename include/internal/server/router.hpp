#pragma once

#include <fstore/model/bucket.hpp>
#include <fstore/model/file.hpp>
#include <fstore/model/object.hpp>
#include <fstore/model/remove_result.hpp>
#include <fstore/model/server_info.hpp>
#include <fstore/net/zipline/socket.hpp>

namespace fstore::core {
    class object_store;
}

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
        ) -> ext::task<object>;

        auto fetch_bucket(std::string bucket_name) -> ext::task<bucket>;

        auto get_object(
            UUID::uuid bucket_id,
            UUID::uuid object_id
        ) -> ext::task<file>;

        auto get_object_metadata(
            UUID::uuid bucket_id,
            UUID::uuid object_id
        ) -> ext::task<object>;

        auto get_server_info() -> ext::task<server_info>;

        auto remove_object(
            UUID::uuid bucket_id,
            UUID::uuid object_id
        ) -> ext::task<object>;

        auto remove_objects(
            UUID::uuid bucket_id,
            std::vector<UUID::uuid> objects
        ) -> ext::task<remove_result>;
    };
}
