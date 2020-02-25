#include <service.h>

namespace fstore::service {
    std::unique_ptr<object_store> object_store::get() {
        return std::make_unique<object_store_core>();
    }

    std::unique_ptr<bucket> object_store_core::create_bucket(
        std::string_view name
    ) const {
        return std::make_unique<bucket_core>(name);
    }

    std::optional<std::unique_ptr<bucket>> object_store_core::fetch_bucket(
        std::string_view name
    ) const {
        try {
            return std::make_unique<bucket_core>(repo::db::fetch_bucket(name));
        }
        catch (const fstore::core::fstore_error& ex) {
            return {};
        }
    }

    std::vector<std::unique_ptr<bucket>> object_store_core::fetch_buckets()
    const {
        auto bucket_entities = repo::db::fetch_buckets();
        std::vector<std::unique_ptr<bucket>> buckets;

        for (auto& entity : bucket_entities)
            buckets.push_back(std::make_unique<bucket_core>(std::move(entity)));

        return buckets;
    }

    std::vector<std::unique_ptr<bucket>> object_store_core::fetch_buckets(
        const std::vector<std::string>& names
    ) const {
        auto bucket_entities = repo::db::fetch_buckets(names);
        std::vector<std::unique_ptr<bucket>> buckets;

        for (auto& entity : bucket_entities)
            buckets.push_back(std::make_unique<bucket_core>(std::move(entity)));

        return buckets;
    }

    core::store_totals object_store_core::get_store_totals() const {
        return repo::db::get_store_totals();
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
