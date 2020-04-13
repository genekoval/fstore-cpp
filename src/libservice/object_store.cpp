#include "service.h"

#include <fstore/error.h>
#include <fstore/repo/filesystem.h>

#include <cstdlib>

namespace fstore::service {
    object_store::object_store(const settings& config) :
        entity(config.connection_string),
        fs(repo::fs::get(config.objects_dir))
    {}

    std::unique_ptr<core::bucket> object_store::create_bucket(
        const std::string& name
    ) {
        entity.create_bucket(name);
        return std::make_unique<bucket>(entity.fetch_bucket(name), fs);
    }

    std::optional<std::unique_ptr<core::bucket>> object_store::fetch_bucket(
        const std::string& name
    ) {
        try {
            return std::make_unique<bucket>(entity.fetch_bucket(name), fs);
        }
        catch (const fstore_error& ex) {
            return {};
        }
    }

    std::vector<std::unique_ptr<core::bucket>> object_store::fetch_buckets() {
        auto entities = entity.fetch_buckets();
        std::vector<std::unique_ptr<core::bucket>> buckets;

        for (auto& entity : entities)
            buckets.push_back(std::make_unique<bucket>(std::move(entity), fs));

        return buckets;
    }

    std::vector<std::unique_ptr<core::bucket>> object_store::fetch_buckets(
        const std::vector<std::string>& names
    ) {
        auto entities = entity.fetch_buckets(names);
        std::vector<std::unique_ptr<core::bucket>> buckets;

        for (auto& entity : entities)
            buckets.push_back(std::make_unique<bucket>(std::move(entity), fs));

        return buckets;
    }

    std::unique_ptr<core::store_totals> object_store::get_store_totals() {
        return std::make_unique<repo::db::store_totals>(
            std::move(entity.get_store_totals())
        );
    }

    std::vector<std::unique_ptr<core::object>> object_store::prune() {
        std::vector<std::unique_ptr<core::object>> orphans;

        for (const auto& orphan : entity.delete_orphan_objects()) {
            orphans.push_back(
                std::make_unique<object>(std::move(orphan))
            );

            fs->remove_object(orphan.id());
        }

        return orphans;
    }
}
