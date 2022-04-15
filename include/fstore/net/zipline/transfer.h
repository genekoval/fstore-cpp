#pragma once

#include <fstore/client.h>
#include <fstore/core/model.h>

#include <uuid++/zipline>
#include <zipline/zipline>

namespace zipline {
    template <typename Socket>
    struct transfer<Socket, fstore::blob> {
        static auto read(Socket& sock) -> fstore::blob {
            auto stream = transfer<Socket, data_stream<Socket>>::read(sock);

            auto blob = fstore::blob(stream.size());
            auto index = 0;

            stream.read([&blob, &index](auto&& chunk) {
                blob.copy(chunk.data(), chunk.size(), index);
                index += chunk.size();
            });

            return blob;
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
    struct transfer<Socket, fstore::core::file> {
        static auto write(
            Socket& sock,
            const fstore::core::file& file
        ) -> void {
            transfer<Socket, decltype(fstore::core::file::size)>::write(
                sock,
                file.size
            );

            sock.flush();
            sock.socket().sendfile(file.fd, file.size);
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
