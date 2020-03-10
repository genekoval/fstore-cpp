#include <service.h>

#include <fstore/repo/filesystem.h>

namespace fstore::service {
    std::unique_ptr<object_store> object_store::get() {
        return std::make_unique<object_store_core>();
    }

    std::unique_ptr<bucket> object_store_core::create_bucket(
        std::string_view name
    ) const {
        entity.create_bucket(name);
        return std::make_unique<bucket_core>(entity.fetch_bucket(name));
    }

    std::optional<std::unique_ptr<bucket>> object_store_core::fetch_bucket(
        std::string_view name
    ) const {
        try {
            return std::make_unique<bucket_core>(entity.fetch_bucket(name));
        }
        catch (const fstore::core::fstore_error& ex) {
            return {};
        }
    }

    std::vector<std::unique_ptr<bucket>> object_store_core::fetch_buckets()
    const {
        auto entities = entity.fetch_buckets();
        std::vector<std::unique_ptr<bucket>> buckets;

        for (auto& entity : entities)
            buckets.push_back(std::make_unique<bucket_core>(std::move(entity)));

        return buckets;
    }

    std::vector<std::unique_ptr<bucket>> object_store_core::fetch_buckets(
        const std::vector<std::string>& names
    ) const {
        auto entities = entity.fetch_buckets(names);
        std::vector<std::unique_ptr<bucket>> buckets;

        for (auto& entity : entities)
            buckets.push_back(std::make_unique<bucket_core>(std::move(entity)));

        return buckets;
    }

    std::unique_ptr<core::store_totals> object_store_core::get_store_totals()
    const {
        return std::make_unique<repo::db::store_totals>(
            std::move(entity.get_store_totals())
        );
    }

    std::vector<std::unique_ptr<object>> object_store_core::prune() const {
        std::vector<std::unique_ptr<object>> orphans;

        for (const auto& orphan : entity.delete_orphan_objects()) {
            orphans.push_back(
                std::make_unique<object_core>(std::move(orphan))
            );

            repo::fs::remove_from_store(orphan.id());
        }

        return orphans;
    }
}
