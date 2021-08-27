#pragma once

#include <fstore/client.h>
#include <fstore/service/model.h>

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
        fstore::service::bucket,
        &fstore::service::bucket::id,
        &fstore::service::bucket::name,
        &fstore::service::bucket::date_created,
        &fstore::service::bucket::size,
        &fstore::service::bucket::space_used
    );

    template <typename Socket>
    struct transfer<Socket, fstore::service::file> {
        static auto write(
            Socket& sock,
            const fstore::service::file& file
        ) -> void {
            transfer<Socket, decltype(fstore::service::file::size)>::write(
                sock,
                file.size
            );

            sock.flush();
            sock.socket().sendfile(file.fd, file.size);
        }
    };

    ZIPLINE_OBJECT(
        fstore::service::object,
        &fstore::service::object::id,
        &fstore::service::object::hash,
        &fstore::service::object::size,
        &fstore::service::object::mime_type,
        &fstore::service::object::date_added
    );
}
