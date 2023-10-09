#pragma once

#include "object.hpp"

#include <fstore/net/zipline.hpp>

namespace fstore {
    struct file {
        object metadata;
        netcore::fd fd;
    };
}

namespace zipline {
    template <fstore::net::writer Writer>
    struct encoder<fstore::file, Writer> {
        static auto encode(const fstore::file& file, Writer& writer)
            -> ext::task<> {
            co_await zipline::encode(file.metadata.size, writer);
            co_await writer.flush();
            co_await writer.sendfile(file.fd, file.metadata.size);
        }
    };
}
