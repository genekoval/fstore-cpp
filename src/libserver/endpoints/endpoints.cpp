#include "endpoints.h"

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

    auto remove_object(protocol& proto) -> void {
        auto bucket_id = proto.read<std::string>();
        auto object_id = proto.read<std::string>();

        proto.reply(proto.store->remove_object(bucket_id, object_id));
    }
}
