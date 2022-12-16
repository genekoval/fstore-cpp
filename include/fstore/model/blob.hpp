#pragma once

#include <ext/dynarray.h>
#include <zipline/zipline>

namespace fstore {
    using blob = ext::dynarray<std::byte>;
}

namespace zipline {
    template <io::reader Reader>
    struct decoder<fstore::blob, Reader> {
        static auto decode(Reader& reader) -> ext::task<fstore::blob> {
            auto stream =
                co_await zipline::decode<zipline::stream<Reader>>(reader);

            auto blob = fstore::blob(co_await stream.size());
            auto index = 0;

            co_await stream.read([&blob, &index](
                std::span<const std::byte> chunk
            ) -> ext::task<> {
                blob.copy(chunk.data(), chunk.size(), index);
                index += chunk.size();
                co_return;
            });

            co_return blob;
        }
    };
}
