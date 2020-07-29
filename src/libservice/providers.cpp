#include "service.h"

#include <timber/timber>

namespace fstore::service {
    auto connect(
        std::string_view db,
        std::string_view fs
    ) -> std::unique_ptr<core::object_store> {
        INFO() << "Database connection: " << db;
        INFO() << "Object storage: " << std::filesystem::canonical(fs);

        return std::make_unique<object_store>(db, fs);
    }

    auto local_store(
        const settings& config
    ) -> std::unique_ptr<core::object_store> {
        return std::make_unique<object_store>(config);
    }
}
