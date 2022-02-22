#include "api.h"

namespace fstore::cli {
    api_container::api_container(std::string_view confpath) :
        api_container(conf::settings::load_file(confpath))
    {}

    api_container::api_container(const fstore::conf::settings& config) :
        database(config.database.connection.str()),
        fs(config.home),
        store(database, fs)
    {}

    auto api_container::object_store() -> core::object_store& {
        return store;
    }
}
