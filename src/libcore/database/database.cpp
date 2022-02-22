#include <fstore/error.h>
#include <fstore/core/db/database.h>
#include <fstore/core/db/entity.h>

#include <ext/string.h>
#include <pqxx/pqxx>

namespace fstore::core::db {
    database::database(std::string_view connection_string) :
        connection(std::string(connection_string))
    {
        auto c = entix::connection(connection);

        c.prepare("add_object", {"uuid", "uuid", "text", "bigint", "text"});
        c.prepare("create_bucket", {"uuid", "text"});
        c.prepare("fetch_bucket", {"text"});
        c.prepare("fetch_buckets", {"text[]"});
        c.prepare("fetch_buckets_all", {});
        c.prepare("fetch_store_totals", {});
        c.prepare("get_object", {"uuid", "uuid"});
        c.prepare("remove_bucket", {"uuid"});
        c.prepare("remove_object", {"uuid", "uuid"});
        c.prepare("remove_objects", {"uuid", "uuid[]"});
        c.prepare("remove_orphan_objects", {});
        c.prepare("rename_bucket", {"uuid", "text"});
    }

    auto database::add_object(
        std::string_view bucket_id,
        std::string_view object_id,
        std::string_view hash,
        uintmax_t size,
        std::string mime_type
    ) -> object {
        auto tx = ntx();
        return entix::make_entity<object>(
            tx,
            __FUNCTION__,
            bucket_id,
            object_id,
            hash,
            size,
            mime_type
        );
    }

    auto database::add_object(
        std::string_view bucket_id,
        const object& obj
    ) -> void {
        add_object(
            bucket_id,
            obj.id,
            obj.hash,
            obj.size,
            obj.mime_type
        );
    }

    auto database::create_bucket(
            std::string_view bucket_id,
            std::string_view name
    ) -> bucket {
        auto tx = ntx();

        try {
            return entix::make_entity<bucket>(
                tx,
                __FUNCTION__,
                bucket_id,
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
        auto tx = ntx();

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
        auto tx = ntx();
        return entix::make_entities<std::vector<bucket>>(
            tx,
            "fetch_buckets_all"
        );
    }

    auto database::fetch_buckets(
        const std::vector<std::string>& names
    ) -> std::vector<bucket> {
        auto tx = ntx();
        return entix::make_entities<std::vector<bucket>>(
            tx,
            __FUNCTION__,
            names
        );
    }

    auto database::fetch_store_totals() -> store_totals {
        auto tx = ntx();
        return entix::make_entity<store_totals>(tx, __FUNCTION__);
    }

    auto database::get_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> std::optional<object> {
        auto tx = ntx();

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

    auto database::ntx() -> pqxx::nontransaction {
        return pqxx::nontransaction(connection);
    }

    auto database::remove_bucket(std::string_view bucket_id) -> void {
        ntx().exec_prepared(__FUNCTION__, bucket_id);
    }

    auto database::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> object {
        auto tx = ntx();

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
        std::string_view bucket_id,
        const std::vector<std::string>& objects
    ) -> remove_result {
        auto tx = ntx();
        return entix::make_entity<remove_result>(
            tx,
            __FUNCTION__,
            bucket_id,
            objects
        );
    }

    auto database::remove_orphan_objects() -> std::vector<object> {
        auto tx = ntx();
        return entix::make_entities<std::vector<object>>(tx, __FUNCTION__);
    }

    auto database::rename_bucket(
        std::string_view id,
        std::string_view name
    ) -> void {
        try {
            ntx().exec_prepared(__FUNCTION__, id, name);
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore_error(
                "cannot rename bucket: bucket named "
                QUOTE_VIEW(name) " already exists"
            );
        }
    }
}
