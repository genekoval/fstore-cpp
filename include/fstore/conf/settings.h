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

        struct s_database {
            std::string client = "psql";
            conftools::database_connection connection;
        };

        static auto load(std::string_view text) -> settings;

        static auto load_file(std::string_view path) -> settings;

        s_daemon daemon;
        s_database database;
        std::string objects_dir;
        netcore::unix_socket server;
    };
}
