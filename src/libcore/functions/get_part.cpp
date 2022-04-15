#include "object_store.h"

namespace fstore::core {
    auto object_store::get_part(
        std::optional<UUID::uuid> part_id
    ) -> part {
        const auto id = part_id.value_or(generate_uuid());
        return part(id, filesystem->get_part(id));
    }
}
