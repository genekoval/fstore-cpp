#pragma once

#include <pqxx/pqxx>
#include <stdexcept>

namespace repo {
    namespace db {
        struct entity_exists_exception : std::runtime_error {
            entity_exists_exception(const std::string& message);
        };

        namespace bucket {
            bool create(const std::string& name);
            bool remove(const std::string& name);
        }
    }
}
