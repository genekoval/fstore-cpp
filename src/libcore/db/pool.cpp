#include <internal/core/db/connection.hpp>
#include <internal/core/db/pool.hpp>

namespace fstore::core::db {
    provider::provider(const pg::parameters& params) : params(params) {}

    auto provider::provide() -> ext::task<pg::client> {
        auto client = co_await pg::connect(params);

        co_await client.prepare_fn("add_object", &connection::add_object);
        co_await client.prepare_fn("create_bucket", &connection::create_bucket);
        co_await client.prepare_fn("fetch_bucket", &connection::fetch_bucket);
        co_await client.prepare_fn(
            "fetch_buckets",
            static_cast<
                ext::task<std::vector<bucket>> (connection::*)(
                    std::span<const std::string>
                )
            >(&connection::fetch_buckets)
        );
        co_await client.prepare_fn(
            "fetch_buckets_all",
            static_cast<
                ext::task<std::vector<bucket>> (connection::*)()
            >(&connection::fetch_buckets)
        );
        co_await client.prepare_fn(
            "fetch_store_totals",
            &connection::fetch_store_totals
        );
        co_await client.prepare_fn("get_errors", &connection::get_errors);
        co_await client.prepare_fn("get_object", &connection::get_object);
        co_await client.prepare("get_objects", "SELECT * FROM get_objects()");
        co_await client.prepare_fn("remove_bucket", &connection::remove_bucket);
        co_await client.prepare_fn("remove_object", &connection::remove_object);
        co_await client.prepare_fn(
            "remove_objects",
            &connection::remove_objects
        );
        co_await client.prepare_fn(
            "remove_orphan_objects",
            &connection::remove_orphan_objects
        );
        co_await client.prepare_fn("rename_bucket", &connection::rename_bucket);
        co_await client.prepare_fn("update_object_errors",
            &connection::update_object_errors
        );

        co_return client;
    }
}
