#include <database.h>

#include <fstore/repo.h>

#include <string>
#include <uuid/uuid.h>

using std::string;
using std::string_view;
using std::uintmax_t;

namespace fstore::repo::db {
    namespace bucket {
        void create(const uuid& id, const string& name) {
            pqxx::work transaction(connect());

            transaction.exec_params(
                "SELECT storage.create_bucket($1, $2)",
                id,
                name
            );
            transaction.commit();
        }

        void remove(const string& name) {
            pqxx::work transaction(connect());

            transaction.exec_params1(
                "SELECT storage.delete_bucket($1)",
                name
            )[0].as<bool>();
            transaction.commit();
        }
    }

    namespace object {
        void add(
            const string& bucket,
            const uuid& id,
            const string& checksum,
            uintmax_t size
        ) {
            pqxx::work transaction(connect());

            transaction.exec_params(
                "SELECT storage.add_object($1, ("
                    "SELECT storage.create_object($2, $3, $4)"
                "))",
                bucket,
                id,
                checksum,
                size
            );
            transaction.commit();
        }
    }
}
