#include <database.h>

#include <fstore/core.h>
#include <fstore/repo.h>
#include <nova/ext/string.h>

namespace fstore::repo::db {
    std::string add_object(
        std::string_view bucket_id,
        std::string_view object_id,
        std::string_view object_hash,
        uintmax_t object_size
    ) {
        pqxx::work transaction(connect());

        auto row = transaction.exec_params1(
            "SELECT add_object($1, ("
                "SELECT create_object($2, $3, $4)"
            "))",
            std::string(bucket_id),
            std::string(object_id),
            std::string(object_hash),
            object_size
        );

        transaction.commit();
        return row[0].as<std::string>();
    }

    void create_bucket(
        std::string_view bucket_id,
        std::string_view bucket_name
    ) {
        pqxx::work transaction(connect());

        try {
            transaction.exec_params(
                "SELECT create_bucket($1, $2)",
                std::string(bucket_id),
                std::string(bucket_name)
            );
            transaction.commit();
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
        pqxx::work transaction(connect());

        transaction.exec_params(
            "SELECT delete_bucket($1)",
            std::string(bucket_id)
        );

        transaction.commit();
    }

    std::vector<object> delete_orphan_objects() {
        pqxx::work transaction(connect());

        auto rows = transaction.exec_params(
            "SELECT id, hash, len "
            "FROM delete_orphan_objects()"
        );
        transaction.commit();

        std::vector<object> deleted_objects;

        for (const auto& row : rows)
            deleted_objects.push_back(object{
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                row[2].as<uintmax_t>()
            });

        return deleted_objects;

    }

    std::string fetch_bucket(std::string_view bucket_name) {
        pqxx::work transaction(connect());

        try {
            auto row = transaction.exec_params1(
                "SELECT id "
                "FROM bucket "
                "WHERE name = $1",
                std::string(bucket_name)
            );

            return row[0].as<std::string>();
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore::core::fstore_error(
                "failed to fetch bucket " QUOTE_VIEW(bucket_name) ": "
                "bucket does not exist"
            );
        }
    }

    object remove_object(
        std::string_view bucket_id,
        std::string_view object_id
    ) {
        pqxx::work transaction(connect());

        try {
            auto row = transaction.exec_params1(
                "SELECT id, hash, len "
                "FROM remove_object($1, $2)",
                std::string(bucket_id),
                std::string(object_id)
            );
            transaction.commit();

            return object{
                row[0].as<std::string>(),
                row[1].as<std::string>(),
                row[2].as<uintmax_t>()
            };
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore::core::fstore_error("bucket does not contain object");
        }
    }

    void rename_bucket(
        std::string_view bucket_id,
        std::string_view new_name
    ) {
        pqxx::work transaction(connect());

        try {
            transaction.exec_params(
                "SELECT rename_bucket($1, $2)",
                std::string(bucket_id),
                std::string(new_name)
            );
            transaction.commit();
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
