#pragma once

#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace fstore::service {
    struct settings {
        const std::string connection_string;
        const std::string objects_dir;
        const std::string unix_socket;

        settings();
        settings(const std::filesystem::path& config);
        settings(const YAML::Node& node);
    };
}
