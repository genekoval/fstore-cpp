#pragma once

#include "object_store.hpp"

#include <ext/pool>

namespace fstore {
    class client {
        class provider {
            std::string path;
        public:
            provider() = default;

            provider(std::string_view path);

            auto provide() -> ext::task<object_store>;
        };

        ext::async_pool<object_store, provider> pool;
    public:
        client() = default;

        client(std::string_view path);

        auto connect() -> ext::task<ext::pool_item<object_store>>;
    };
}
