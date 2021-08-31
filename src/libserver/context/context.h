#pragma once

#include <fstore/net/zipline/protocol.h>
#include <fstore/core/object_store.h>

namespace fstore::server {
    class context {
        core::object_store* store;
    public:
        context(core::object_store& store);

        auto add_object(
            std::string bucket_id,
            std::optional<std::string> request,
            net::data_stream stream
        ) -> core::object;

        auto create_object_from_file(
            std::string bucket_id,
            std::string path
        ) -> core::object;

        auto fetch_bucket(std::string bucket_name) -> core::bucket;

        auto get_object(
            std::string bucket_id,
            std::string object_id
        ) -> core::file;

        auto get_object_metadata(
            std::string bucket_id,
            std::string object_id
        ) -> core::object;

        auto remove_object(
            std::string bucket_id,
            std::string object_id
        ) -> core::object;

        auto remove_objects(
            std::string bucket_id,
            std::vector<std::string> objects
        ) -> core::remove_result;
    };
}
