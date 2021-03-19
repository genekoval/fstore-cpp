#pragma once

#include <netcore/netcore>
#include <zipline/zipline>

namespace fstore::net {
    constexpr auto buffer_size = 8192;

    using event_t = unsigned int;

    using socket = zipline::buffered_socket<netcore::socket, buffer_size>;
    using data_stream = zipline::data_stream<socket>;
}
