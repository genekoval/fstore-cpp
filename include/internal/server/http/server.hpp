#pragma once

#include <internal/core/object_store.hpp>

#include <fstore/model/server_info.hpp>

#include <conftools/endpoint.hpp>
#include <http/http>

namespace fstore::server::http {
    auto listen(
        ::http::server::router& router,
        std::string_view cert,
        std::string_view key,
        std::span<const conftools::endpoint> endpoints
    ) -> ext::task<::http::server::server_list>;

    auto router(const server_info& info, core::object_store& store)
        -> ::http::server::router;
}
