#include <internal/core/db/database.hpp>

#include <ext/string.h>

namespace fstore::core::db {
    database::database(const pg::parameters& params) : pool(provider(params)) {}

#ifdef TEST
    auto database::connect() -> ext::task<connection_wrapper> {
        co_return connection_wrapper(std::shared_ptr<connection>(
            new connection(co_await pool.checkout())
        ));
    }
#else
    auto database::connect() -> ext::task<connection> {
        co_return connection(co_await pool.checkout());
    }
#endif

    database::provider::provider(const pg::parameters& params) :
        params(params)
    {}

    auto database::provider::provide() -> ext::task<pg::client> {
        auto client = co_await pg::connect(params);

        co_await client.prepare_fn("add_error", &connection::add_error);
        co_await client.prepare_fn("add_object", &connection::add_object);
        co_await client.prepare_fn("clear_error", &connection::clear_error);
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

        co_return client;
    }
}
