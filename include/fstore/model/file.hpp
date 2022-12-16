#pragma once

#include <fstore/net/zipline/writer.hpp>

namespace fstore {
    struct file {
        netcore::fd fd;
        std::size_t size;
    };
}

namespace zipline {
    template <fstore::net::writer Writer>
    struct encoder<fstore::file, Writer> {
        static auto encode(
            const fstore::file& file,
            Writer& writer
        ) -> ext::task<> {
            co_await zipline::encode(file.size, writer);
            co_await writer.flush();
            co_await writer.sendfile(file.fd, file.size);
        }
    };
}
