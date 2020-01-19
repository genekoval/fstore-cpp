#pragma once

#include <pqxx/pqxx>

namespace fstore::repo::db {
    pqxx::connection& connect();
}
