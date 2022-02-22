#pragma once

#include <fstore/conf/settings.h>
#include <fstore/core/filesystem.h>
#include <fstore/core/object_store.h>

namespace fstore::cli {
    class api_container {
        fstore::core::filesystem fs;
        fstore::core::object_store store;
    public:
        api_container(std::string_view confpath);

        api_container(const fstore::conf::settings& config);

        auto object_store() -> core::object_store&;
    };
}
