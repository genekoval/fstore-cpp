#include "object_store.h"

namespace fstore::core {
    auto object_store::commit_part(
        const UUID::uuid& bucket_id,
        const UUID::uuid& part_id
    ) -> object {
        auto part = filesystem->part_path(part_id);

        const auto mime = filesystem->mime_type(part);

        const auto obj = database->add_object(
            bucket_id,
            part_id,
            filesystem->hash(part),
            filesystem->size(part),
            mime.type,
            mime.subtype
        );

        // This object was uploaded previously.
        if (obj.id != part_id) filesystem->remove_part(part_id);
        else filesystem->make_object(part_id);

        return obj;
    }
}
