#include "api.hpp"

namespace fstore::cli {
    api_container::api_container(std::string_view confpath) :
        api_container(conf::settings::load_file(confpath))
    {}

    api_container::api_container(const fstore::conf::settings& config) :
        params(pg::parameters::parse(config.database.connection.parameters)),
        database(params),
        fs(config.home),
        store(database, fs)
    {}

    auto api_container::object_store() -> core::object_store& {
        return store;
    }
}
