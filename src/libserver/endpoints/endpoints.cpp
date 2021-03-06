#include "endpoints.h"

#include <fstore/error.h>
#include <fstore/server/transfer.h>

#include <span>

namespace fstore::server::endpoint {
    auto add_object(protocol& proto) -> void {
        auto bucket_id = proto.read<std::string>();
        auto request = proto.read<std::optional<std::string>>();
        auto stream = proto.read<zipline::data_stream<netcore::socket>>();

        auto part_id = std::string();

        {
            auto part = proto.store->get_part(request);
            part_id = part.id;
            stream.read([&part](auto&& chunk) {
                part.write(chunk);
            });
        }

        auto object = proto.store->commit_part(bucket_id, part_id);
        proto.reply(object);
    }

    auto create_object_from_file(protocol& proto) -> void {
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
