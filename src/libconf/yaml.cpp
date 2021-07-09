#include "yaml.h"

using s = fstore::conf::settings;

namespace YAML {
    auto convert<s>::decode(
        const Node& node,
        s& settings
    ) -> bool {
        settings.daemon = node["daemon"].as<decltype(s::daemon)>();
        settings.database = node["database"].as<decltype(s::database)>();
        settings.objects_dir = node["objects"].as<decltype(s::objects_dir)>();
        settings.server = node["server"].as<decltype(s::server)>();

        return true;
    }

    auto convert<s::s_daemon>::decode(
        const Node& node,
        s::s_daemon& daemon
    ) -> bool {
        daemon.group = node["group"].as<decltype(s::s_daemon::group)>();
        daemon.pidfile = node["pidfile"].as<decltype(s::s_daemon::pidfile)>();
        daemon.user = node["user"].as<decltype(s::s_daemon::user)>();

        return true;
    }
}
