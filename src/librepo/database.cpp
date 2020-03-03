#include <database.h>

#include <algorithm>

namespace fstore::repo::db {
    pqxx::connection& connect() {
        static pqxx::connection connection(
            "postgresql://fstore@localhost/fstore"
        );

        return connection;
    }

    std::vector<std::string> quote(
        const std::vector<std::string>& elements,
        const pqxx::transaction_base& transaction
    ) {
        std::vector<std::string> quoted(elements.size());

        std::transform(elements.begin(), elements.end(), quoted.begin(),
            [&transaction](const std::string& element) -> std::string {
                return transaction.quote(element);
            }
        );

        return quoted;
    }
}
