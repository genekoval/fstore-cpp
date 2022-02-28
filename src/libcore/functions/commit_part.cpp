#include "object_store.h"

namespace fstore::core {
    auto object_store::commit_part(
        std::string_view bucket_id,
        std::string_view part_id
    ) -> object {
        auto part = filesystem->part_path(part_id);

        const auto obj = database->add_object(
            bucket_id,
            part_id,
            filesystem->hash(part),
            filesystem->size(part),
            filesystem->mime_type(part)
        );

        // This object was uploaded previously.
        if (obj.id != part_id) filesystem->remove_part(part_id);
        else filesystem->make_object(part_id);

        return obj;
    }
}
