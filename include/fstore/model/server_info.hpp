#pragma once

#include <zipline/zipline>

namespace fstore {
    struct server_info {
        std::string version;
    };
}

namespace zipline {
    ZIPLINE_OBJECT(
        fstore::server_info,
        &fstore::server_info::version
    );
}
