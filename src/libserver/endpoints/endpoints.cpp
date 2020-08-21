#include "endpoints.h"

#include <fstore/error.h>
#include <fstore/server/transfer.h>

namespace fstore::server::endpoint {
    auto add_object(protocol& proto) -> void {
        auto bucket_id = proto.read<std::string>();
        auto path = proto.read<std::string>();

        proto.reply(proto.store->add_object(bucket_id, path));
    }

    auto fetch_bucket(protocol& proto) -> void {
        auto name = proto.read<std::string>();
        proto.reply(proto.store->fetch_bucket(name));
    }

    auto get_object(protocol& proto) -> void {
        auto bucket_id = proto.read<std::string>();
        auto object_id = proto.read<std::string>();

        const auto obj = proto.store->get_object(bucket_id, object_id);
        if (!obj) throw fstore_error("bucket does not contain object");

        const auto& [metadata, file] = *obj;

        proto.reply(metadata.mime_type);
        proto.sendfile(file, metadata.size);
    }

    auto get_object_metadata(protocol& proto) -> void {
        auto bucket_id = proto.read<std::string>();
        auto object_id = proto.read<std::string>();

        const auto obj = proto.store->get_object_metadata(bucket_id, object_id);
        if (!obj) throw fstore_error("bucket does not contain object");

        proto.reply(*obj);
    }

    auto remove_object(protocol& proto) -> void {
        auto bucket_id = proto.read<std::string>();
        auto object_id = proto.read<std::string>();

        proto.reply(proto.store->remove_object(bucket_id, object_id));
    }
}
