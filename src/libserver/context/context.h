#pragma once

#include <fstore/net/zipline/protocol.h>
#include <fstore/service/object_store.h>

namespace fstore::server {
    class context {
        service::object_store* store;
    public:
        context(service::object_store& store);

        auto add_object(
            std::string bucket_id,
            std::optional<std::string> request,
            net::data_stream stream
        ) -> service::object;

        auto create_object_from_file(
            std::string bucket_id,
            std::string path
        ) -> service::object;

        auto fetch_bucket(std::string bucket_name) -> service::bucket;

        auto get_object(
            std::string bucket_id,
            std::string object_id
        ) -> service::file;

        auto get_object_metadata(
            std::string bucket_id,
            std::string object_id
        ) -> service::object;

        auto remove_object(
            std::string bucket_id,
            std::string object_id
        ) -> service::object;
    };
}
