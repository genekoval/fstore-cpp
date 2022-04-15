#include "object_store.h"

namespace fstore::core {
    auto object_store::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> std::optional<file> {
        auto meta = get_object_metadata(bucket_id, object_id);
        if (!meta) return {};

        return file { filesystem->open(object_id), meta.value().size };
    }
}
