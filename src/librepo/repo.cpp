#include <database.h>

#include <fstore/core.h>
#include <fstore/repo.h>

using entix::query;

namespace fstore::repo::db {
    std::string add_object(
        std::string_view bucket_id,
        std::string_view object_id,
        std::string_view object_hash,
        uintmax_t object_size,
        std::string_view object_mime_type
    ) {
        auto tx = pqxx::nontransaction(connect());

        return tx.exec_params1(
            query{}
                .select(
                    "add_object($1, ("
                        "SELECT create_object($2, $3, $4, $5)"
                    "))"
                )
            .str(),
            std::string(bucket_id),
            std::string(object_id),
            std::string(object_hash),
            object_size,
            std::string(object_mime_type)
        )[0].as<std::string>();
    }

    void create_bucket(
        std::string_view bucket_id,
        std::string_view bucket_name
    ) {
        auto tx = pqxx::nontransaction(connect());

        try {
            tx.exec_params(
                query{}
                    .select("create_bucket($1, $2)")
                .str(),
                std::string(bucket_id),
                std::string(bucket_name)
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw core::fstore_error(
                "cannot create bucket "
                QUOTE_VIEW(bucket_name)
                ": bucket exists"
            );
        }
        catch (const pqxx::check_violation& ex) {
            throw fstore::core::fstore_error(
                "cannot create bucket: bucket name empty"
            );
        }
    }

    void delete_bucket(std::string_view bucket_id) {
        auto tx = pqxx::nontransaction(connect());

        tx.exec_params(
            query{}
                .select("delete_bucket($1)")
            .str(),
            std::string(bucket_id)
        );
    }

    std::vector<object> delete_orphan_objects() {
        auto tx = pqxx::nontransaction(connect());

        return object::create_from(tx.exec_params(
            query{}
                .select<object>()
                .from("delete_orphan_objects()")
            .str()
        ));
    }

    bucket fetch_bucket(std::string_view bucket_name) {
        auto tx = pqxx::nontransaction(connect());

        try {
            return bucket(tx.exec_params1(
                query{}
                    .select<bucket>()
                    .from("bucket_view")
                    .where(bucket::column<bucket::c_name>())
                        .equals("$1")
                .str(),
                std::string(bucket_name)
            ));
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore::core::fstore_error(
                "failed to fetch bucket " QUOTE_VIEW(bucket_name) ": "
                "bucket does not exist"
            );
        }
    }

    std::vector<bucket> fetch_buckets() {
        auto tx = pqxx::nontransaction(connect());

        return bucket::create_from(tx.exec(
            query{}
                .select<bucket>()
                .from("bucket_view")
                .order_by_desc(bucket::column<bucket::c_object_count>())
            .str()
        ));
    }

    std::vector<bucket> fetch_buckets(const std::vector<std::string>& names) {
        auto tx = pqxx::nontransaction(connect());

        return bucket::create_from(tx.exec(
            query{}
                .select<bucket>()
                .from("bucket_view")
                .where(bucket::column<bucket::c_name>())
                    .in(quote(names, tx))
                .order_by_asc(bucket::column<bucket::c_name>())
            .str()
        ));
    }

    std::unique_ptr<core::store_totals> get_store_totals() {
        auto tx = pqxx::nontransaction(connect());

        return std::make_unique<store_totals>(tx.exec1(
            query{}
                .select<store_totals>()
                .from("store_totals")
            .str()
        ));
    }

    object remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) {
        auto tx = pqxx::nontransaction(connect());

        try {
            return object(tx.exec_params1(
                query{}
                    .select<object>()
                    .from("remove_object($1, $2)")
                .str(),
                std::string(bucket_id),
                std::string(object_id)
            ));
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore::core::fstore_error("bucket does not contain object");
        }
    }

    void rename_bucket(
        std::string_view bucket_id,
        std::string_view new_name
    ) {
        auto tx = pqxx::nontransaction(connect());

        try {
            tx.exec_params(
                query{}
                    .select("rename_bucket($1, $2)")
                .str(),
                std::string(bucket_id),
                std::string(new_name)
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore::core::fstore_error(
                "cannot rename bucket: bucket named "
                QUOTE_VIEW(new_name) " already exists"
            );
        }
        catch (const pqxx::check_violation& ex) {
            throw fstore::core::fstore_error(
                "cannot rename bucket: name empty"
            );
        }
    }
}
