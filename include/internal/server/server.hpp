#pragma once

#include "router.hpp"

#include <conftools/endpoint.hpp>
#include <fstore/event.hpp>
#include <fstore/except.hpp>

namespace fstore::server {
    namespace detail {
        template <typename... Routes>
        auto make_router(router_context&& context, Routes&&... routes) {
            return zipline::router<
                net::socket,
                std::underlying_type_t<event>,
                router_context,
                Routes...>(
                std::forward<router_context>(context),
                error_list::codes(),
                std::forward<Routes>(routes)...
            );
        }
    }

    inline auto make_router(
        core::object_store& store,
        const server_info& info
    ) {
        return detail::make_router(
            router_context(store, info),
            &router_context::add_object,
            &router_context::fetch_bucket,
            &router_context::get_object,
            &router_context::get_object_metadata,
            &router_context::get_server_info,
            &router_context::remove_object,
            &router_context::remove_objects
        );
    }

    using router_type = std::invoke_result_t<
        decltype(make_router),
        core::object_store&,
        const server_info&>;

    class server_context {
        router_type* router;
        seconds timeout;
    public:
        server_context(router_type& router, seconds timeout);

        auto close() -> void;

        auto connection(netcore::socket&& client) -> ext::task<>;

        auto listen(const netcore::address_type& address) -> void;
    };

    using server = netcore::server<server_context>;
    using server_list = netcore::server_list<server_context>;

    auto listen(
        router_type& router,
        std::span<const conftools::endpoint> endpoints
    ) -> ext::task<server_list>;
}
