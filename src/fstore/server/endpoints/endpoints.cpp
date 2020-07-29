#include "endpoints.h"

#include <fstore/core.h>
#include <fstore/server/transfer.h>

namespace fstore::server {
    static auto $get_store_totals(
        core::object_store& store
    ) -> std::unique_ptr<core::store_totals> {
        return store.get_store_totals();
    }

    auto get_store_totals(protocol& proto) -> void {
        proto.use($get_store_totals);
    }
}
