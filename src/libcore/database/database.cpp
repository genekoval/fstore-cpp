#include <fstore/error.h>
#include <fstore/core/db/database.h>
#include <fstore/core/db/entity.h>

#include <ext/string.h>
#include <pqxx/pqxx>

namespace fstore::core::db {
    database::database(
        std::string_view connection_string,
        int connection_count
    ) :
        connections(
            connection_string,
            connection_count,
            [](auto& connection) {
                entix::prepare(connection, "add_error", {"uuid", "text"});
                entix::prepare(
                    connection,
                    "add_object",
                    {"uuid", "uuid", "text", "bigint", "text", "text"}
                );
                entix::prepare(connection, "clear_error", {"uuid"});
                entix::prepare(connection, "create_bucket", {"text"});
                entix::prepare(connection, "fetch_bucket", {"text"});
                entix::prepare(connection, "fetch_buckets", {"text[]"});
                entix::prepare(connection, "fetch_buckets_all", {});
                entix::prepare(connection, "fetch_store_totals", {});
                entix::prepare(connection, "get_errors", {});
                entix::prepare(connection, "get_object", {"uuid", "uuid"});
                entix::prepare(connection, "remove_bucket", {"uuid"});
                entix::prepare(connection, "remove_object", {"uuid", "uuid"});
                entix::prepare(
                    connection,
                    "remove_objects",
                    {"uuid", "uuid[]"}
                );
                entix::prepare(connection, "remove_orphan_objects", {});
                entix::prepare(connection, "rename_bucket", {"uuid", "text"});
            }
        )
    {}

    auto database::add_error(
        const UUID::uuid& object_id,
        std::string_view message
    ) -> void {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        tx.exec_prepared(__FUNCTION__, object_id, message);
    }

    auto database::add_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id,
        std::string_view hash,
        uintmax_t size,
        std::string_view type,
        std::string_view subtype
    ) -> object {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        return entix::make_entity<object>(
            tx,
            __FUNCTION__,
            bucket_id,
            object_id,
            hash,
            size,
            type,
            subtype
        );
    }

    auto database::clear_error(const UUID::uuid& object_id) -> void {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        tx.exec_prepared(__FUNCTION__, object_id);
    }

    auto database::create_bucket(std::string_view name) -> bucket {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        try {
            return entix::make_entity<bucket>(
                tx,
                __FUNCTION__,
                name
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore_error(
                "cannot create bucket " QUOTE_VIEW(name) ": bucket exists"
            );
        }
    }

    auto database::fetch_bucket(std::string_view name) -> bucket {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        try {
            return entix::make_entity<bucket>(tx, __FUNCTION__, name);
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore_error(
                "failed to fetch bucket " QUOTE_VIEW(name) ": "
                "bucket does not exist"
            );
        }
    }

    auto database::fetch_buckets() -> std::vector<bucket> {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        return entix::make_entities<std::vector<bucket>>(
            tx,
            "fetch_buckets_all"
        );
    }

    auto database::fetch_buckets(
        const std::vector<std::string>& names
    ) -> std::vector<bucket> {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        return entix::make_entities<std::vector<bucket>>(
            tx,
            __FUNCTION__,
            names
        );
    }

    auto database::fetch_store_totals() -> store_totals {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        return entix::make_entity<store_totals>(tx, __FUNCTION__);
    }

    auto database::get_errors() -> std::vector<object_error> {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        return entix::make_entities<std::vector<object_error>>(
            tx,
            __FUNCTION__
        );
    }

    auto database::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> std::optional<object> {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        try {
            return entix::make_entity<object>(
                tx,
                __FUNCTION__,
                bucket_id,
                object_id
            );
        }
        catch (const pqxx::unexpected_rows& ex) {
            return {};
        }
    }

    auto database::get_objects(
        int batch_size
    ) -> ext::generator<std::span<object>> {
        auto c = connections.connection();
        auto tx = pqxx::work(c);

        auto stream = entix::stream<object>(tx, __FUNCTION__, batch_size);
        while (stream) co_yield stream();

        tx.commit();
    }

    auto database::remove_bucket(std::string_view bucket_id) -> void {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        tx.exec_prepared(__FUNCTION__, bucket_id);
    }

    auto database::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> object {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        try {
            return entix::make_entity<object>(
                tx,
                __FUNCTION__,
                bucket_id,
                object_id
            );
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore_error("bucket does not contain object");
        }
    }

    auto database::remove_objects(
        const UUID::uuid& bucket_id,
        const std::vector<UUID::uuid>& objects
    ) -> remove_result {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        return entix::make_entity<remove_result>(
            tx,
            __FUNCTION__,
            bucket_id,
            objects
        );
    }

    auto database::remove_orphan_objects() -> std::vector<object> {
        auto c = connections.connection();
        auto tx = pqxx::nontransaction(c);

        return entix::make_entities<std::vector<object>>(tx, __FUNCTION__);
    }

    auto database::rename_bucket(
        const UUID::uuid& id,
        std::string_view name
    ) -> void {
        try {
            auto c = connections.connection();
            auto tx = pqxx::nontransaction(c);

            tx.exec_prepared(__FUNCTION__, id, name);
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore_error(
                "cannot rename bucket: bucket named "
                QUOTE_VIEW(name) " already exists"
            );
        }
    }
}
