#include "object_store.h"

namespace fstore::core {
    auto object_store::prune() -> std::vector<object> {
        auto orphans = database->remove_orphan_objects();
        for (const auto& obj : orphans) filesystem->remove(obj.id);

        INFO() << "Pruned " << orphans.size() << " objects";

        return orphans;
    }
}
