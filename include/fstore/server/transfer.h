#pragma once

#include <fstore/client.h>
#include <fstore/models.h>

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

    template <typename Socket>
    struct transfer<Socket, fstore::model::bucket> {
        static auto read(Socket& sock) -> fstore::model::bucket {
            return fstore::model::bucket {
                .id = transfer<Socket,
                    decltype(fstore::model::bucket::id)>::read(sock),
                .name = transfer<Socket,
                    decltype(fstore::model::bucket::name)>::read(sock),
                .date_created = transfer<Socket,
                    decltype(fstore::model::bucket::date_created)>::read(sock),
                .size = transfer<Socket,
                    decltype(fstore::model::bucket::size)>::read(sock),
                .space_used = transfer<Socket,
                        decltype(fstore::model::bucket::space_used)>::read(sock),
            };
        }

        static auto write(
            Socket& sock,
            const fstore::model::bucket& bucket
        ) -> void {
            transfer<Socket, decltype(fstore::model::bucket::id)>::write(
                sock,
                bucket.id
            );

            transfer<Socket, decltype(fstore::model::bucket::name)>::write(
                sock,
                bucket.name
            );

            transfer<
                Socket,
                decltype(fstore::model::bucket::date_created)
            >::write(
                sock,
                bucket.date_created
            );

            transfer<Socket, decltype(fstore::model::bucket::size)>::write(
                sock,
                bucket.size
            );

            transfer<Socket, decltype(fstore::model::bucket::space_used)>::write(
                sock,
                bucket.space_used
            );
        }
    };

    template <typename Socket>
    struct transfer<Socket, fstore::model::file> {
        static auto write(
            Socket& sock,
            const fstore::model::file& file
        ) -> void {
            transfer<Socket, decltype(fstore::model::file::size)>::write(
                sock,
                file.size
            );

            sock.flush();
            sock.socket().sendfile(file.fd, file.size);
        }
    };

    template <typename Socket>
    struct transfer<Socket, fstore::model::object> {
        static auto read(Socket& sock) -> fstore::model::object{
            return fstore::model::object {
                .id = transfer<Socket,
                    decltype(fstore::model::object::id)>::read(sock),
                .hash = transfer<Socket,
                    decltype(fstore::model::object::hash)>::read(sock),
                .size = transfer<Socket,
                    decltype(fstore::model::object::size)>::read(sock),
                .mime_type = transfer<Socket,
                        decltype(fstore::model::object::mime_type)>::read(sock),
                .date_added = transfer<Socket,
                    decltype(fstore::model::object::date_added)>::read(sock)
            };
        }

        static auto write(
            Socket& sock,
            const fstore::model::object& object
        ) -> void {
            transfer<Socket, decltype(fstore::model::object::id)>::write(
                sock,
                object.id
            );

            transfer<Socket, decltype(fstore::model::object::hash)>::write(
                sock,
                object.hash
            );

            transfer<Socket, decltype(fstore::model::object::size)>::write(
                sock,
                object.size
            );

            transfer<Socket, decltype(fstore::model::object::mime_type)>::write(
                sock,
                object.mime_type
            );

            transfer<
                Socket,
                decltype(fstore::model::object::date_added)
            >::write(
                sock,
                object.date_added
            );
        }
    };
}
