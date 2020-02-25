#include <database.h>

#include <algorithm>
#include <nova/ext/string.h>

namespace fstore::repo::db {
    pqxx::connection& connect() {
        static pqxx::connection connection(
            "postgresql://fstore@localhost/fstore"
        );

        return connection;
    }

    std::string quoted_list(
        const std::vector<std::string>& elements,
        const pqxx::work& transaction
    ) {
        std::vector<std::string> escaped(elements.size());

        std::transform(elements.begin(), elements.end(), escaped.begin(),
            [&transaction](const std::string& element) -> std::string {
                return transaction.quote(element);
            }
        );

        return nova::ext::string::join(escaped, ", ");
    }
}
