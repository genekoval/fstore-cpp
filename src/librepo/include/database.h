#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>

namespace fstore::repo::db {
    pqxx::connection& connect();

    std::string quoted_list(
      const std::vector<std::string>& elements,
      const pqxx::work& transaction
    );
}
