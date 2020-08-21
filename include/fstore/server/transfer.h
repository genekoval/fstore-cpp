#pragma once

#include <fstore/client.h>
#include <fstore/models.h>

#include <zipline/zipline>

namespace zipline {
    template <typename Socket>
    struct transfer<Socket, fstore::model::bucket> {
        static auto read(const Socket& sock) -> fstore::model::bucket {
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
            const Socket& sock,
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
    struct transfer<Socket, fstore::model::object> {
        static auto read(const Socket& sock) -> fstore::model::object{
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
            const Socket& sock,
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

    template <typename Socket>
    struct transfer<Socket, fstore::object_content> {
        static auto read(const Socket& sock) -> fstore::object_content {
            return fstore::object_content {
                .mime_type = transfer<Socket, std::remove_const_t<
                    decltype(fstore::object_content::mime_type)>>::read(sock),
                .data = transfer<Socket, std::remove_const_t<
                    decltype(fstore::object_content::data)>>::read(sock)
            };
        }
    };
}
