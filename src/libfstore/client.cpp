#include <fstore/client.h>

namespace fstore {
    client::client(std::string_view path) : pool(provider(path)) {}

    auto client::connect() -> ext::task<ext::pool_item<object_store>> {
        co_return co_await pool.checkout();
    }

    client::provider::provider(std::string_view path) : path(path) {}

    auto client::provider::provide() -> ext::task<object_store> {
        auto socket = co_await netcore::connect(path);
        co_return object_store(std::move(socket));
    }
}
