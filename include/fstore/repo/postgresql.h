#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>

namespace fstore::repo::db {
    class postgresql {
    public:
        using connection_t = pqxx::connection;
    private:
        connection_t connection;
    public:
        postgresql(const std::string& options);

        auto connect() -> connection_t&;

        auto quote(
            const std::vector<std::string>& elements
        ) -> std::vector<std::string>;
    };
}
