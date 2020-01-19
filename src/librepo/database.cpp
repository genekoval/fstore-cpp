#include <database.h>

namespace fstore::repo::db {
    pqxx::connection& connect() {
        static pqxx::connection connection(
            "postgresql://fstore@localhost/fstore"
        );

        return connection;
    }
}
