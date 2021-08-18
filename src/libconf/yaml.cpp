#include "yaml.h"

namespace c = conftools;

using settings = fstore::conf::settings;

namespace YAML {
    DECODE(
        settings,
        c::required("daemon", &settings::daemon),
        c::required("database", &settings::database),
        c::required("objects", &settings::objects_dir),
        c::required("server", &settings::server)
    );

    DECODE(
        settings::s_daemon,
        c::required("group", &settings::s_daemon::group),
        c::required("pidfile", &settings::s_daemon::pidfile),
        c::required("user", &settings::s_daemon::user)
    );
}
