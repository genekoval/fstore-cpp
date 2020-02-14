#include <service.h>

namespace fstore::service {
    std::unique_ptr<object_store> object_store::get() {
        return std::make_unique<object_store_core>();
    }

    std::vector<std::unique_ptr<object>> object_store_core::prune() const {
        std::vector<std::unique_ptr<object>> orphans;
        object_core::add_entities(orphans, repo::db::delete_orphan_objects());

        for (const auto& orphan : orphans) {
            repo::fs::remove_from_store(orphan->id());
        }

        return orphans;
    }
}
