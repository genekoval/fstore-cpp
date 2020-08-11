#include "endpoints.h"

#include <fstore/server/transfer.h>

namespace fstore::server::endpoint {
    auto $add_object(
        service::object_store& store,
        std::string bucket_id,
        std::string path
    ) -> model::object {
        return store.add_object(bucket_id, path);
    }

    auto $create_bucket(
        service::object_store& store,
        std::string name
    ) -> model::bucket {
        return store.create_bucket(name);
    }

    auto $fetch_bucket(
        service::object_store& store,
        std::string name
    ) -> model::bucket {
        return store.fetch_bucket(name);
    }

    auto $remove_bucket(
        service::object_store& store,
        std::string name
    ) -> void {
        store.remove_bucket(name);
    }

    auto $remove_object(
        service::object_store& store,
        std::string bucket_id,
        std::string object_id
    ) -> model::object {
        return store.remove_object(bucket_id, object_id);
    }

    auto $rename_bucket(
        service::object_store& store,
        std::string bucket_id,
        std::string bucket_name
    ) -> void {
        store.rename_bucket(bucket_id, bucket_name);
    }

// -----------------------------------------------------------------------------

    auto add_object(protocol& proto) -> void {
        proto.use($add_object);
    }

    auto create_bucket(protocol& proto) -> void {
        proto.use($create_bucket);
    }

    auto fetch_bucket(protocol& proto) -> void {
        proto.use($fetch_bucket);
    }

    auto remove_bucket(protocol& proto) -> void {
        proto.use($remove_bucket);
    }

    auto remove_object(protocol& proto) -> void {
        proto.use($remove_object);
    }

    auto rename_bucket(protocol& proto) -> void {
        proto.use($rename_bucket);
    }
}
