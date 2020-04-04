#include <fstore/error.h>
#include <fstore/repo/object_store.h>

#include <uuid++/uuid.h>

using entix::query;

namespace fstore::repo::db {
    object_store::object_store(std::string_view options) :
        connected<postgresql>(
            std::make_shared<postgresql>(std::string(options))
        )
    {}

    void object_store::create_bucket(std::string_view bucket_name) {
        auto tx = pqxx::nontransaction(db->connect());

        auto uuid = UUID::uuid{};
        uuid.generate();

        try {
            tx.exec_params(
                query{}
                    .select("create_bucket($1, $2)")
                .str(),
                uuid.string(),
                std::string(bucket_name)
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore_error(
                "cannot create bucket "
                QUOTE_VIEW(bucket_name)
                ": bucket exists"
            );
        }
        catch (const pqxx::check_violation& ex) {
            throw fstore_error(
                "cannot create bucket: bucket name empty"
            );
        }
    }

    std::vector<object> object_store::delete_orphan_objects() {
        auto tx = pqxx::nontransaction(db->connect());

        return create_entities<object>(tx.exec_params(
            query{}
                .select<object>()
                .from("delete_orphan_objects()")
            .str()
        ));
    }

    bucket object_store::fetch_bucket(std::string_view bucket_name) {
        auto tx = pqxx::nontransaction(db->connect());

        try {
            return create_entity<bucket>(tx.exec_params1(
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
            throw fstore_error(
                "failed to fetch bucket " QUOTE_VIEW(bucket_name) ": "
                "bucket does not exist"
            );
        }
    }

    std::vector<bucket> object_store::fetch_buckets(
        const std::vector<std::string>& names
    ) {
        auto tx = pqxx::nontransaction(db->connect());

        return create_entities<bucket>(tx.exec(
            query{}
                .select<bucket>()
                .from("bucket_view")
                .where(bucket::column<bucket::c_name>())
                    .in(db->quote(names))
                .order_by_asc(bucket::column<bucket::c_name>())
            .str()
        ));
    }

    std::vector<bucket> object_store::fetch_buckets() {
        auto tx = pqxx::nontransaction(db->connect());

        return create_entities<bucket>(tx.exec(
            query{}
                .select<bucket>()
                .from("bucket_view")
                .order_by_desc(bucket::column<bucket::c_object_count>())
            .str()
        ));
    }

    store_totals object_store::get_store_totals() {
        auto tx = pqxx::nontransaction(db->connect());

        return create_entity<store_totals>(tx.exec1(
            query{}
                .select<store_totals>()
                .from("store_totals")
            .str()
        ));
    }

    void object_store::truncate_buckets() {
        auto tx = pqxx::nontransaction(db->connect());

        tx.exec("TRUNCATE bucket CASCADE");
    }

    void object_store::truncate_objects() {
        auto tx = pqxx::nontransaction(db->connect());

        tx.exec("TRUNCATE object CASCADE");
    }
}
