#pragma once

#include <fstore/service/object_store.h>

#include <netcore/socket.h>
#include <string_view>
#include <zipline/zipline>

namespace fstore::server {
    using event_t = unsigned int;

    class connection {
        const netcore::socket* sock;
    public:
        service::object_store* store;

        connection(
            const netcore::socket& sock,
            service::object_store& store
        );

        auto recv(void* buffer, std::size_t len) const -> void;

        auto send(const void* buffer, std::size_t len) const -> void;
    };

    class protocol : public zipline::protocol<connection> {
    public:
        using zipline::protocol<connection>::protocol;

        template <typename R, typename ...Args>
        auto use(R (*callable)(service::object_store&, Args...)) -> void {
            const auto result = callable(*(sock->store), read<Args>()...);

            write(true);
            write(result);
        }

        template <typename ...Args>
        auto use(void (*callable)(service::object_store&, Args...)) -> void {
            callable(*(sock->store), read<Args>()...);

            write(true);
        }
    };

    auto listen(
        service::object_store& store,
        std::string_view endpoint
    ) -> void;
}
