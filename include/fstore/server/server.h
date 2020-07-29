#pragma once

#include <fstore/core.h>

#include <netcore/socket.h>
#include <string_view>
#include <zipline/zipline>

namespace fstore::server {
    using event_t = unsigned int;

    class connection {
        const netcore::socket* sock;
    public:
        core::object_store* store;

        connection(
            const netcore::socket& sock,
            core::object_store& store
        );

        auto recv(void* buffer, std::size_t len) const -> void;

        auto send(const void* buffer, std::size_t len) const -> void;
    };

    class protocol : public zipline::protocol<connection> {
    public:
        using zipline::protocol<connection>::protocol;

        template <typename R>
        auto use(R (*callable)(core::object_store&)) -> void {
            const auto result = callable(*(sock->store));

            write(true);
            write(result);
        }

        auto use(void (*callable)(core::object_store&)) -> void {
            callable(*(sock->store));

            write(true);
        }

        template <typename R, typename ...Args>
        auto use(R (*callable)(core::object_store&, Args...)) -> void {
            const auto result = callable(*(sock->store), (read<Args>(), ...));

            write(true);
            write(result);
        }

        template <typename ...Args>
        auto use(void (*callable)(core::object_store&, Args...)) -> void {
            callable(*(sock->store), (read<Args>(), ...));

            write(true);
        }
    };

    auto listen(
        core::object_store& store,
        std::string_view endpoint
    ) -> void;
}
