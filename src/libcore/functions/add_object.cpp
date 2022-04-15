#include "object_store.h"

namespace fstore::core {
    auto object_store::add_object(
        const UUID::uuid& bucket_id,
        std::string_view path
    ) -> object {
        const auto mime = filesystem->mime_type(path);

        const auto obj = database->add_object(
            bucket_id,
            generate_uuid(),
            filesystem->hash(path),
            filesystem->size(path),
            mime.type,
            mime.subtype
        );

        filesystem->copy(path, obj.id);
        return obj;
    }
}
