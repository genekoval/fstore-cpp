#pragma once

#include <fstore/conf/settings.h>

#include <conftools/yaml.h>

namespace YAML {
    template <>
    struct convert<fstore::conf::settings> {
        static auto decode(
            const Node& node,
            fstore::conf::settings& settings
        ) -> bool;
    };

    template <>
    struct convert<fstore::conf::settings::s_daemon> {
        static auto decode(
            const Node& node,
            fstore::conf::settings::s_daemon& daemon
        ) -> bool;
    };
}
