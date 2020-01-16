#pragma once

#include <util.h>

#include <pqxx/pqxx>
#include <stdexcept>
#include <string>

namespace repo {
    namespace db {
        namespace bucket {
            bool create(const std::string& name);
            bool remove(const std::string& name);
        }

        namespace object {
            util::uuid::uuid add(
                const std::string& bucket,
                const std::string& checksum,
                std::uintmax_t size
            );
        }
    }
}
