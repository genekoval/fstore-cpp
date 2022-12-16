#include "yaml.hpp"

namespace fstore::conf {
    auto settings::load(std::string_view text) -> settings {
        return YAML::Load(text.data()).as<settings>();
    }

    auto settings::load_file(std::string_view path) -> settings {
        return YAML::LoadFile(path.data()).as<settings>();
    }
}
