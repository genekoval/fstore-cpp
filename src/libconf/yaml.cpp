#include "yaml.h"

namespace c = conftools;

using settings = fstore::conf::settings;

namespace YAML {
    DECODE(
        settings,
        c::optional("archive", &settings::archive),
        c::required("daemon", &settings::daemon),
        c::required("database", &settings::database),
        c::required("home", &settings::home),
        c::required("server", &settings::server)
    );

    DECODE(
        settings::s_archive,
        c::optional("options", &settings::s_archive::options),
        c::required("path", &settings::s_archive::path),
        c::optional("sync", &settings::s_archive::sync)
    );

    DECODE(
        settings::s_daemon,
        c::required("group", &settings::s_daemon::group),
        c::required("pidfile", &settings::s_daemon::pidfile),
        c::required("user", &settings::s_daemon::user)
    );

    DECODE(
        settings::s_database,
        c::optional("client", &settings::s_database::client),
        c::required("connection", &settings::s_database::connection),
        c::optional("connections", &settings::s_database::connections),
        c::optional("dump", &settings::s_database::dump),
        c::optional("restore", &settings::s_database::restore),
        c::required("sqldir", &settings::s_database::sqldir)
    );
}
