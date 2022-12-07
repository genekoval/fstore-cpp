#pragma once

#include <ext/data_size.h>
#include <netcore/netcore>
#include <zipline/zipline>

namespace fstore::net {
    using namespace ext::literals;

    using event_t = unsigned int;

    using socket = zipline::io::buffered<netcore::socket, 8_KiB>;
    using data_stream = zipline::stream<socket>;

    using error_list = zipline::error_list<socket>;
}
