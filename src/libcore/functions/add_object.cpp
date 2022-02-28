#include "object_store.h"

namespace fstore::core {
    auto object_store::add_object(
        std::string_view bucket_id,
        std::string_view path
    ) -> object {
        auto uuid = UUID::uuid();
        uuid.generate();

        const auto obj = database->add_object(
            bucket_id,
            uuid.string(),
            filesystem->hash(path),
            filesystem->size(path),
            filesystem->mime_type(path)
        );

        filesystem->copy(path, obj.id);
        return obj;
    }
}
