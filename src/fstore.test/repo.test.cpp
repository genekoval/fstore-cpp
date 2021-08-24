#include <fstore/test.h>

#include <uuid++/uuid.h>

constexpr auto test_db = "postgresql://fstore@localhost/fstore";

namespace fstore::test {
    auto db_exec(const std::string& query) -> void {
        auto connection = pqxx::connection(test_db);
        auto tx = pqxx::nontransaction(connection);

        tx.exec(query);
    }

    auto db() -> repo::db {
        return repo::db(test_db);
    }

    auto create_bucket(repo::db& db, std::string_view name) -> model::bucket {
        auto uuid = UUID::uuid();
        uuid.generate();

        auto bucket = model::bucket {
            .id = uuid.string(),
            .name = std::string(name)
        };

        db.create_bucket(bucket);
        return bucket;
    }

    auto drop_buckets() -> void {
        db_exec("truncate data.bucket cascade");
    }

    auto drop_objects() -> void {
        db_exec("truncate data.object cascade");
    }
}
