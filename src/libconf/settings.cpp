#include <fstore/conf/settings.h>

#include <conftools/yaml.h>

namespace YAML {
    using settings = fstore::conf::settings;

    template <>
    struct convert<settings> {
        static auto decode(const Node& node, settings& s) -> bool {
            s.database = node["database"]
                .as<decltype(settings::database)>();
            s.objects_dir = node["objects"]
                .as<decltype(settings::objects_dir)>();
            s.server = node["server"]
                .as<decltype(settings::server)>();

            return true;
        }
    };
}

namespace fstore::conf {
    auto settings::load(std::string_view text) -> settings {
        return YAML::Load(text.data()).as<settings>();
    }

    auto settings::load_file(std::string_view path) -> settings {
        return YAML::LoadFile(path.data()).as<settings>();
    }
}
