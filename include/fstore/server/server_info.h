#pragma once

#include <string>
#include <zipline/zipline>

namespace fstore::server {
    struct server_info {
        std::string version;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::server::server_info,
        &fstore::server::server_info::version
    );
}
