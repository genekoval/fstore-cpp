#pragma once

#include <fstore/core.h>

#include <zipline/zipline>

namespace zipline {
    template <typename Socket>
    struct transfer<Socket, std::unique_ptr<fstore::core::store_totals>> {
        static auto write(
            const Socket& sock,
            const std::unique_ptr<fstore::core::store_totals>& totals
        ) -> void {
            transfer<Socket, decltype(totals->bucket_count())>::write(
                sock,
                totals->bucket_count()
            );

            transfer<Socket, decltype(totals->object_count())>::write(
                sock,
                totals->object_count()
            );

            transfer<Socket, decltype(totals->space_used())>::write(
                sock,
                totals->space_used()
            );
        }
    };
}
