#pragma once

#include <conftools/database.h>
#include <ext/unix.h>
#include <netcore/netcore>

namespace fstore::conf {
    struct settings {
        struct s_daemon {
            ext::group group;
            std::string pidfile;
            ext::user user;
        };

        static auto load(std::string_view text) -> settings;

        static auto load_file(std::string_view path) -> settings;

        s_daemon daemon;
        conftools::database_connection database;
        std::string objects_dir;
        netcore::unix_socket server;
    };
}
