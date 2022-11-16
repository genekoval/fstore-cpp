#pragma once

#include <ext/data_size.h>
#include <netcore/netcore>
#include <zipline/zipline>

namespace fstore::net {
    using namespace ext::literals;

    using event_t = unsigned int;

    using socket = zipline::buffered_socket<netcore::socket, 8_KiB>;
    using data_stream = zipline::data_stream<socket>;

    using error_list = zipline::error_list<socket>;
}
