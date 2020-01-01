#include <repo.h>

#include <string>
#include <uuid/uuid.h>

using std::runtime_error;
using std::string;
using std::string_view;

namespace repo::db {
    entity_exists_exception::entity_exists_exception(const string& message) :
        runtime_error(message)
    {}

    string generate_uuid() {
        uuid_t uuid;
        uuid_generate(uuid);

        char uuid_str[37]; // 32 hex digits + 4 hyphens + 1 null character
        uuid_unparse(uuid, uuid_str);

        return uuid_str;
    }

    pqxx::connection& connect() {
        static pqxx::connection connection(
            "postgresql://fstore@localhost/fstore"
        );

        return connection;
    }
}

namespace repo::db::bucket {
    void create(const string& name) {
        pqxx::work transaction(connect());

        try {
            transaction.exec_params(
                "SELECT storage.create_bucket($1, $2)",
                generate_uuid(),
                name
            );

            transaction.commit();
        }
        catch (const pqxx::unique_violation& ex) {
            throw entity_exists_exception(
                "bucket named '" + name + "' already exists"
            );
        }
    }
}
