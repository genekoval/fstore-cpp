#pragma once

#include <fstore/net/zipline/protocol.h>
#include <fstore/service/object_store.h>

#include <string_view>

namespace fstore::server {
    struct protocol : zipline::protocol<net::socket> {
        service::object_store* store;

        protocol(
            net::socket& sock,
            service::object_store& store
        );

        auto sendfile(const netcore::fd& fd, std::size_t count) -> void;
    };

    auto listen(
        service::object_store& store,
        std::string_view endpoint,
        const std::function<void()>& callback
    ) -> void;
}
