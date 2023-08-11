#include <internal/server/http/server.hpp>

using conftools::endpoint;
using http::server::server_list;

namespace {
    auto on_error(
        const endpoint& endpoint,
        std::exception_ptr exception
    ) -> void {
        try {
            if (exception) std::rethrow_exception(exception);
        }
        catch (const std::exception& ex) {
            TIMBER_ERROR(ex.what());
        }
    }
}

namespace fstore::server::http {
    auto listen(
        ::http::server::router& router,
        std::string_view cert,
        std::string_view key,
        std::span<const endpoint> endpoints
    ) -> ext::task<server_list> {
        using endpoint_ref =
            std::optional<std::reference_wrapper<const netcore::endpoint>>;

        auto ssl = ::http::server::ssl();
        ssl.certificate(cert);
        ssl.private_key(key);

        co_return co_await server_list::listen(
            endpoints,
            [&](const endpoint& endpoint, endpoint_ref& out) {
                out.emplace(endpoint.endpoint);
                return ::http::server::server(ssl, router);
            },
            on_error
        );
    }
}
