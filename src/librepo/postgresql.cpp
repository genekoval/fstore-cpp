#include <fstore/repo/postgresql.h>

#include <algorithm>

namespace fstore::repo::db {
    postgresql::postgresql(const std::string& url) :
        connection(std::make_shared<pqxx::connection>(url))
    {}

    auto postgresql::connect() -> connection_t& { return *connection; }

    auto postgresql::quote(
        const std::vector<std::string>& elements
    ) -> std::vector<std::string> {
        auto quoted = std::vector<std::string>(elements.size());

        std::transform(elements.begin(), elements.end(), quoted.begin(),
            [this](const std::string& element) -> std::string {
                return connection->quote(element);
            }
        );

        return quoted;
    }
}
