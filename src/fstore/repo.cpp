#include <repo.h>

#include <string>
#include <uuid/uuid.h>

using std::runtime_error;
using std::string;
using std::string_view;
using std::uintmax_t;

namespace repo::db {
    uuid generate_uuid() {
        uuid_t uuid_obj;
        uuid_generate(uuid_obj);

        char uuid_str[37]; // 32 hex digits + 4 hyphens + 1 null character
        uuid_unparse(uuid_obj, uuid_str);

        return uuid_str;
    }

    pqxx::connection& connect() {
        static pqxx::connection connection(
            "postgresql://fstore@localhost/fstore"
        );

        return connection;
    }

    namespace bucket {
        bool create(const string& name) {
            pqxx::work transaction(connect());

            try {
                transaction.exec_params(
                    "SELECT storage.create_bucket($1, $2)",
                    generate_uuid(),
                    name
                );
                transaction.commit();

                return true;
            }
            catch (const pqxx::unique_violation& ex) {
                return false;
            }
        }

        bool remove(const string& name) {
            pqxx::work transaction(connect());

            auto deleted = transaction.exec_params1(
                "SELECT storage.delete_bucket($1)",
                name
            )[0].as<bool>();
            transaction.commit();

            return deleted;
        }
    }

    namespace object {
        uuid add(
            const string& bucket,
            const string& checksum,
            uintmax_t size
        ) {
            pqxx::work transaction(connect());
            auto obj_id = generate_uuid();

            transaction.exec_params(
                "SELECT storage.add_object($1, ("
                    "SELECT storage.create_object($2, $3, $4)"
                "))",
                bucket,
                obj_id,
                checksum,
                size
            );
            transaction.commit();

            return obj_id;
        }
    }
}
