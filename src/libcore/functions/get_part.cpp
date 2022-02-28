#include "object_store.h"

namespace fstore::core {
    auto object_store::get_part(
        std::optional<std::string_view> part_id
    ) -> part {
        std::string id;

        if (part_id) id = *part_id;
        else {
            auto uuid = UUID::uuid();
            uuid.generate();
            id = uuid.string();
        }

        return part(id, filesystem->get_part(id));
    }
}
