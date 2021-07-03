#pragma once

#include <netcore/netcore>

namespace fstore::conf {
    struct settings {
        static auto load(std::string_view text) -> settings;

        static auto load_file(std::string_view path) -> settings;

        std::string database;
        std::string objects_dir;
        netcore::unix_socket server;
    };
}
