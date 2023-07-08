#include <internal/core/db/connection.hpp>

#include <fstore/except.hpp>

namespace fstore::core::db {
    connection::connection(pool::item&& client) :
        client(std::forward<pool::item>(client))
    {}

    auto connection::add_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id,
        std::string_view hash,
        std::int64_t size,
        std::string_view type,
        std::string_view subtype
    ) -> ext::task<object> {
        co_return co_await client->fetch_prepared<object>(
            __FUNCTION__,
            bucket_id,
            object_id,
            hash,
            size,
            type,
            subtype
        );
    }

    auto connection::begin() -> ext::task<pg::transaction> {
        co_return co_await client->begin();
    }

    auto connection::create_bucket(std::string_view name) -> ext::task<bucket> {
        try {
            co_return co_await client->fetch_prepared<bucket>(
                __FUNCTION__,
                name
            );
        }
        catch (const pg::sql_error& error) {
            if (error.sqlstate(pg::sqlstate::unique_violation)) {
                throw unique_bucket_violation(name);
            }
            else throw;
        }
    }

    auto connection::fetch_bucket(std::string_view name) -> ext::task<bucket> {
        try {
            co_return co_await client->fetch_prepared<bucket>(
                __FUNCTION__,
                name
            );
        }
        catch (const pg::unexpected_data&) {
            throw not_found("bucket '{}' not found", name);
        }
    }

    auto connection::fetch_buckets() -> ext::task<std::vector<bucket>> {
        co_return co_await client->fetch_rows_prepared<bucket>(
            "fetch_buckets_all"
        );
    }

    auto connection::fetch_buckets(
        std::span<const std::string> names
    ) -> ext::task<std::vector<bucket>> {
        co_return co_await client->fetch_rows_prepared<bucket>(
            __FUNCTION__,
            names
        );
    }

    auto connection::fetch_store_totals() -> ext::task<store_totals> {
        co_return co_await client->fetch_prepared<store_totals>(__FUNCTION__);
    }

    auto connection::get_errors() -> ext::task<std::vector<object_error>> {
        co_return co_await client->fetch_rows_prepared<object_error>(
            __FUNCTION__
        );
    }

    auto connection::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        try {
            co_return co_await client->fetch_prepared<object>(
                __FUNCTION__,
                bucket_id,
                object_id
            );
        }
        catch (const pg::unexpected_data&) {
            throw not_found(
                "bucket ({}) does not contain object ({})",
                bucket_id,
                object_id
            );
        }
    }

    auto connection::get_objects(
        std::string_view portal,
        int batch_size
    ) -> ext::task<pg::portal<object>> {
        co_return co_await client->stream_prepared<object>(
            portal,
            __FUNCTION__,
            batch_size
        );
    }

    auto connection::remove_bucket(const UUID::uuid& id) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, id);
    }

    auto connection::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        try {
            co_return co_await client->fetch_prepared<object>(
                __FUNCTION__,
                bucket_id,
                object_id
            );
        }
        catch (const pg::unexpected_data&) {
            throw not_found(
                "bucket ({}) does not contain object ({})",
                bucket_id,
                object_id
            );
        }
    }

    auto connection::remove_objects(
        const UUID::uuid& bucket_id,
        const std::vector<UUID::uuid>& objects
    ) -> ext::task<remove_result> {
        co_return co_await client->fetch_prepared<remove_result>(
            __FUNCTION__,
            bucket_id,
            objects
        );
    }

    auto connection::remove_orphan_objects() -> ext::task<std::vector<object>> {
        co_return co_await client->fetch_rows_prepared<object>(__FUNCTION__);
    }

    auto connection::rename_bucket(
        const UUID::uuid& id,
        std::string_view name
    ) -> ext::task<> {
        try {
            co_await client->query_prepared(__FUNCTION__, id, name);
        }
        catch (const pg::sql_error& error) {
            if (error.sqlstate(pg::sqlstate::unique_violation)) {
                throw unique_bucket_violation(name);
            }
            else throw;
        }
    }

    auto connection::update_object_errors(
        std::span<const object_error> records
    ) -> ext::task<> {
        co_await client->query_prepared(__FUNCTION__, records);
    }
}
