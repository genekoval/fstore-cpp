#include <fstore/error.h>
#include <fstore/repo/db.h>

#include <ext/string.h>
#include <pqxx/pqxx>

namespace fstore::repo {
    auto get_bucket(const pqxx::row& row) -> model::bucket {
        return {
            .id = row[0].as<decltype(model::bucket::id)>(),
            .name = row[1].as<decltype(model::bucket::name)>(),
            .date_created = row[2].as<decltype(model::bucket::date_created)>(),
            .size = row[3].as<decltype(model::bucket::size)>(),
            .space_used = row[4].as<decltype(model::bucket::space_used)>()
        };
    }

    auto set_bucket(model::bucket& bkt, const pqxx::row& row) -> void {
        bkt.id = row[0].as<decltype(model::bucket::id)>();
        bkt.name = row[1].as<decltype(model::bucket::name)>();
        bkt.date_created = row[2].as<decltype(model::bucket::date_created)>();
        bkt.size = row[3].as<decltype(model::bucket::size)>();
        bkt.space_used = row[4].as<decltype(model::bucket::space_used)>();
    }

    auto set_object(model::object& obj, const pqxx::row& row) -> void {
        obj.id = row[0].as<decltype(model::object::id)>();
        obj.hash = row[1].as<decltype(model::object::hash)>();
        obj.size = row[2].as<decltype(model::object::size)>();
        obj.mime_type = row[3].as<decltype(model::object::mime_type)>();
        obj.date_added = row[4].as<decltype(model::object::date_added)>();
    }

    db::db(std::string_view connection_string) :
        connection(std::string(connection_string))
    {}

    auto db::add_object(
        std::string_view bucket_id,
        model::object& obj
    ) -> void {
        auto tx = pqxx::nontransaction(connection);

        set_object(obj, tx.exec_params1(
            "select * from add_object($1, $2, $3, $4, $5)",
            std::string(bucket_id),
            obj.id,
            obj.hash,
            obj.size,
            obj.mime_type
        ));
    }

    auto db::create_bucket(model::bucket& bkt) -> void {
        auto tx = pqxx::nontransaction(connection);

        try {
            auto row = tx.exec_params1(
                "select * from create_bucket($1, $2)",
                bkt.id,
                bkt.name
            );

            bkt.date_created =
                row[2].as<decltype(model::bucket::date_created)>();
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore_error(
                "cannot create bucket " QUOTE(bkt.name) ": bucket exists"
            );
        }
    }

    auto db::fetch_bucket(std::string_view bucket_name) -> model::bucket {
        auto tx = pqxx::nontransaction(connection);

        try {
            return get_bucket(tx.exec_params1(
                "select * from fetch_bucket($1)",
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

    auto db::fetch_buckets() -> std::vector<model::bucket> {
        auto tx = pqxx::nontransaction(connection);

        auto rows = tx.exec("select * from fetch_buckets()");

        auto buckets = std::vector<model::bucket>(rows.size());
        for (auto i = decltype(rows)::size_type(0); i < rows.size(); i++)  {
            set_bucket(buckets[i], rows[i]);
        }

        return buckets;
    }

    auto db::fetch_buckets(
        const std::vector<std::string_view>& names
    ) -> std::vector<model::bucket> {
        auto tx = pqxx::nontransaction(connection);

        auto esc = std::vector<std::string>();

        for (const auto& name : names) {
            esc.push_back(tx.esc(name.data(), name.size()));
        }

        auto array = std::string("{");
        array += ext::join(esc, ",");
        array += "}";

        auto rows = tx.exec_params(
            "select * from fetch_buckets($1)",
            array
        );

        auto buckets = std::vector<model::bucket>(rows.size());
        for (auto i = decltype(rows)::size_type(0); i < rows.size(); i++) {
            set_bucket(buckets[i], rows[i]);
        }

        return buckets;
    }

    auto db::fetch_store_totals() -> model::object_store {
        auto tx = pqxx::nontransaction(connection);

        auto row = tx.exec1("select * from fetch_store_totals()");

        return {
            .buckets = row[0].as<decltype(model::object_store::buckets)>(),
            .objects = row[1].as<decltype(model::object_store::objects)>(),
            .space_used = row[2].as<decltype(model::object_store::space_used)>()
        };
    }

    auto db::remove_bucket(std::string_view bucket_id) -> void {
        auto tx = pqxx::nontransaction(connection);

        tx.exec_params(
            "select * from remove_bucket($1)",
            std::string(bucket_id)
        );
    }

    auto db::remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) -> model::object {
        auto tx = pqxx::nontransaction(connection);

        try {
            auto obj = model::object();

            set_object(obj, tx.exec_params1(
                "select * from remove_object($1, $2)",
                std::string(bucket_id),
                std::string(object_id)
            ));

            return obj;
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore_error("bucket does not contain object");
        }
    }

    auto db::remove_orphan_objects() -> std::vector<model::object> {
        auto tx = pqxx::nontransaction(connection);

        auto rows = tx.exec_params("select * from remove_orphan_objects()");

        auto objects = std::vector<model::object>(rows.size());
        for (auto i = decltype(rows)::size_type(0); i < rows.size(); i++) {
            set_object(objects[i], rows[i]);
        }

        return objects;
    }

    auto db::rename_bucket(
        std::string_view bucket_id,
        std::string_view bucket_name
    ) -> void {
        auto tx = pqxx::nontransaction(connection);

        try {
            tx.exec_params(
                "select * from rename_bucket($1, $2)",
                std::string(bucket_id),
                std::string(bucket_name)
            );
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore_error(
                "cannot rename bucket: bucket named "
                QUOTE_VIEW(bucket_name) " already exists"
            );
        }
    }
}
