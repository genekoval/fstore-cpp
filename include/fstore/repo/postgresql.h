#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>

namespace fstore::repo::db {
    class postgresql {
    public:
        using connection_t = pqxx::connection_base;
    private:
        std::shared_ptr<connection_t> connection;
    public:
        postgresql(const std::string& url);

        auto connect() -> connection_t&;

        auto quote(
            const std::vector<std::string>& elements
        ) -> std::vector<std::string>;
    };
}
