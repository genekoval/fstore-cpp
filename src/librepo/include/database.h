#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>

namespace fstore::repo::db {
    pqxx::connection& connect();

    std::vector<std::string> quote(
      const std::vector<std::string>& elements,
      const pqxx::transaction_base& transaction
    );
}
