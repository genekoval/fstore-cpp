#include <fstore/error.h>
#include <fstore/service/settings.h>

#include <ext/string.h>

#ifndef CONFDIR
#   define CONFDIR "/etc"
#endif

namespace fstore::service {
    static const auto default_config_file =
        std::filesystem::path(CONFDIR) / "fstore" / "fstore.conf";

    static auto extract_string(
        const YAML::Node& node,
        const std::string& key,
        const std::function<const YAML::Node(
            const YAML::Node&,
            const std::string
        )> preprocessor
    ) -> std::string {
        try {
            return ext::expand_env(preprocessor(node, key).as<std::string>());
        }
        catch (const std::invalid_argument& ex) {
            throw fstore_error(
                "configuration for " QUOTE(key) " is invalid: " +
                    std::string(ex.what())
            );
        }
    }

    static auto required(
        const YAML::Node& node,
        const std::string& key
    ) -> YAML::Node {
        if (node[key]) return node[key];

        throw fstore_error(
            "configuration invalid: missing required option: " + key
        );
    }

    static auto load_file(const std::filesystem::path& filename) -> YAML::Node {
        if (!std::filesystem::is_regular_file(filename)) throw fstore_error(
            "configuration file does not exist: " + filename.string()
        );

        return YAML::LoadFile(filename);
    }

    settings::settings() : settings(default_config_file) {}

    settings::settings(const std::filesystem::path& filename) :
        settings(load_file(filename))
    {}

    settings::settings(const YAML::Node& node) :
        connection_string(
            extract_string(node, "database", required)
        ),
        objects_dir(
            extract_string(node, "objects", required)
        ),
        unix_socket(
            extract_string(node, "unix socket", required)
        )
    {}
}
