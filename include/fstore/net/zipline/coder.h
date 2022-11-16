#pragma once

#include <fstore/core/model.h>

#include <ext/dynarray.h>
#include <uuid++/zipline>
#include <zipline/zipline>

namespace fstore {
    using blob = ext::dynarray<std::byte>;
}

namespace zipline {
    template <typename Socket>
    struct coder<Socket, fstore::blob> {
        static auto decode(Socket& sock) -> ext::task<fstore::blob> {
            auto stream =
                co_await coder<Socket, data_stream<Socket>>::decode(sock);

            auto blob = fstore::blob(co_await stream.size());
            auto index = 0;

            co_await stream.read([&blob, &index](auto&& chunk) -> ext::task<> {
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

    template <typename Socket>
    struct coder<Socket, fstore::core::file> {
        static auto encode(
            Socket& sock,
            const fstore::core::file& file
        ) -> ext::task<> {
            co_await coder<Socket, decltype(fstore::core::file::size)>::encode(
                sock,
                file.size
            );

            co_await sock.flush();
            co_await sock.inner().sendfile(file.fd, file.size);
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
