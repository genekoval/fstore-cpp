#pragma once

#include "protocol.h"

#include <fstore/core/model.h>

#include <ext/dynarray.h>
#include <uuid++/zipline>
#include <zipline/zipline>

namespace fstore {
    using blob = ext::dynarray<std::byte>;
}

namespace zipline {
    template <io::reader Reader>
    struct decoder<fstore::blob, Reader> {
        static auto decode(Reader& reader) -> ext::task<fstore::blob> {
            auto stream =
                co_await zipline::decode<zipline::stream<Reader>>(reader);

            auto blob = fstore::blob(co_await stream.size());
            auto index = 0;

            co_await stream.read([&blob, &index](
                std::span<const std::byte> chunk
            ) -> ext::task<> {
                blob.copy(chunk.data(), chunk.size(), index);
                index += chunk.size();
                co_return;
            });

            co_return blob;
        }
    };

    ZIPLINE_OBJECT(
        fstore::core::bucket,
        &fstore::core::bucket::id,
        &fstore::core::bucket::name,
        &fstore::core::bucket::date_created,
        &fstore::core::bucket::size,
        &fstore::core::bucket::space_used
    );

    template <fstore::net::writer Writer>
    struct encoder<fstore::core::file, Writer> {
        static auto encode(
            const fstore::core::file& file,
            Writer& writer
        ) -> ext::task<> {
            co_await zipline::encode(file.size, writer);
            co_await writer.flush();
            co_await writer.sendfile(file.fd, file.size);
        }
    };

    ZIPLINE_OBJECT(
        fstore::core::object,
        &fstore::core::object::id,
        &fstore::core::object::hash,
        &fstore::core::object::size,
        &fstore::core::object::type,
        &fstore::core::object::subtype,
        &fstore::core::object::date_added
    );

    ZIPLINE_OBJECT(
        fstore::core::remove_result,
        &fstore::core::remove_result::objects_removed,
        &fstore::core::remove_result::space_freed
    );
}
