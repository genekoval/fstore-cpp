#pragma once

#include <fstore/types.h>

namespace fstore::repo {
    namespace db {
        namespace bucket {
            void create(const uuid& id, const std::string& name);
            void remove(const std::string& name);
        }

        namespace object {
            void add(
                const std::string& bucket,
                const uuid& id,
                const std::string& checksum,
                std::uintmax_t size
            );
        }
    }
}
