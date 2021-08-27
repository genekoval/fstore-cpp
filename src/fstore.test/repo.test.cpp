#include <fstore/test.h>

#include <uuid++/uuid.h>

constexpr auto test_db = "postgresql://fstore@localhost/fstore";

namespace fstore::test {
    auto db_exec(const std::string& query) -> void {
        auto connection = pqxx::connection(test_db);
        auto tx = pqxx::nontransaction(connection);

        tx.exec(query);
    }

    auto db() -> repo::database {
        return repo::database(test_db);
    }

    auto create_bucket(
        repo::database& db,
        std::string_view name
    ) -> repo::bucket {
        auto uuid = UUID::uuid();
        uuid.generate();

        return db.create_bucket(uuid.string(), name);
    }

    auto drop_buckets() -> void {
        db_exec("truncate data.bucket cascade");
    }

    auto drop_objects() -> void {
        db_exec("truncate data.object cascade");
    }
}
