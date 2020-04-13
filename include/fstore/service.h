#pragma once

#include <fstore/core.h>

#include <yaml-cpp/yaml.h>

namespace fstore::service {
    struct settings {
        const std::string connection_string;
        const std::filesystem::path objects_dir;

        settings();
        settings(const std::filesystem::path& config);
        settings(const YAML::Node& node);
    };

    /**
     * Returns an Object Store implementation that allows for manipulating
     * data directly.
     *
     * @param config Settings that the Object Store instance should use.
     *
     * @returns An Object Store instance for manipulating local data.
     */
    auto local_store(
        const settings& config
    ) -> std::unique_ptr<core::object_store>;
}
