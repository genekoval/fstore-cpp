#pragma once

#include <conftools/database.h>
#include <ext/unix.h>
#include <netcore/netcore>

namespace fstore::conf {
    struct settings {
        struct s_archive {
            std::vector<std::string> options;
            std::string path;
            std::string sync = "rsync";
        };

        struct s_daemon {
            ext::group group;
            std::string pidfile;
            ext::user user;
        };

        struct s_database {
            std::optional<std::string> client;
            conftools::database_connection connection;
            int connections = 1;
            std::optional<std::string> dump;
            std::optional<std::string> restore;
            std::string sqldir;
        };

        static auto load(std::string_view text) -> settings;

        static auto load_file(std::string_view path) -> settings;

        s_archive archive;
        s_daemon daemon;
        s_database database;
        std::string home;
        netcore::unix_socket server;
    };
}
