#pragma once

#include <nlohmann/json.hpp>
#include <zipline/zipline>

namespace fstore {
    struct server_info {
        std::string version;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        server_info,
        version
    );
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::server_info,
        &fstore::server_info::version
    );
}
