#pragma once

#include <fstore/conf/settings.h>

#include <conftools/yaml.h>

namespace YAML {
    CONVERT(fstore::conf::settings);
    CONVERT(fstore::conf::settings::s_daemon);
    CONVERT(fstore::conf::settings::s_database);
}
