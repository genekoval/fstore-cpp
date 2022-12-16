#pragma once

#include <internal/conf/settings.hpp>

#include <conftools/yaml.h>

namespace YAML {
    CONVERT(fstore::conf::settings);
    CONVERT(fstore::conf::settings::s_archive);
    CONVERT(fstore::conf::settings::s_daemon);
    CONVERT(fstore::conf::settings::s_database);
}
