#include "api.h"

namespace fstore::cli {
    api_container::api_container(std::string_view confpath) :
        api_container(conf::settings::load_file(confpath))
    {}

    api_container::api_container(const fstore::conf::settings& config) :
        fs(config.home),
        store(
            fstore::repo::database(config.database.connection.str()),
            fs
        )
    {}

    auto api_container::object_store() -> core::object_store& {
        return store;
    }
}
