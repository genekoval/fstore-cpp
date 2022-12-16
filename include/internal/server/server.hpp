#pragma once

#include "router.hpp"

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
                Routes...
            >(
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
        const server_info&
    >;

    class server_context {
        router_type router;
        timber::timer& startup_timer;
        timber::timer& uptime_timer;
    public:
        server_context(
            router_type&& router,
            timber::timer& startup_timer,
            timber::timer& uptime_timer
        );

        auto close() -> void;

        auto connection(netcore::socket&& client) -> ext::task<>;

        auto listen() -> void;
    };

    using server_type = netcore::server<server_context>;

    auto create(
        core::object_store& store,
        const server_info& info,
        timber::timer& startup_timer,
        timber::timer& uptime_timer
    ) -> server_type;
}
