#include "context.h"

#include <fstore/error.h>
#include <fstore/net/zipline/protocol.h>

#include <span>

namespace fstore::server {
    context::context(core::object_store& store) : store(&store) {}

    auto context::add_object(
        std::string bucket_id,
        std::optional<std::string> request,
        net::data_stream stream
    ) -> core::object {
        auto part_id = std::string();

        {
            auto part = store->get_part(request);
            part_id = part.id;
            stream.read([&part](auto&& chunk) -> void {
                part.write(chunk);
            });
        }

        return store->commit_part(bucket_id, part_id);
    }


    auto context::create_object_from_file(
        std::string bucket_id,
        std::string path
    ) -> core::object {
        return store->add_object(bucket_id, path);
    }

    auto context::fetch_bucket(std::string bucket_name) -> core::bucket {
        return store->fetch_bucket(bucket_name);
    }

    auto context::get_object(
        std::string bucket_id,
        std::string object_id
    ) -> core::file {
        auto file = store->get_object(bucket_id, object_id);
        if (!file) throw fstore_error("bucket does not contain object");

        return std::move(*file);
    }

    auto context::get_object_metadata(
        std::string bucket_id,
        std::string object_id
    ) -> core::object {
        auto object = store->get_object_metadata(bucket_id, object_id);
        if (!object) throw fstore_error("bucket does not contain object");

        return std::move(*object);
    }

    auto context::remove_object(
        std::string bucket_id,
        std::string object_id
    ) -> core::object {
        return store->remove_object(bucket_id, object_id);
    }

    auto context::remove_objects(
        std::string bucket_id,
        std::vector<std::string> objects
    ) -> core::remove_result {
        return store->remove_objects(bucket_id, objects);
    }
}
