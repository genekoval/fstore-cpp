#pragma once

#include <fstore/core.h>
#include <fstore/repo/bucket.h>
#include <fstore/repo/filesystem.h>
#include <fstore/repo/object.h>
#include <fstore/repo/object_store.h>

namespace fstore::service {
    class bucket : public core::bucket {
        repo::db::bucket entity;
        repo::fs::fs_t fs;
    public:
        bucket(
            const repo::db::bucket&& entity,
            repo::fs::fs_t fs
        );

        std::unique_ptr<core::object> add_object(
            const std::filesystem::path& file
        ) override;
        void drop() override;
        const std::string& name() const override;
        void name(const std::string& new_name) override;
        int object_count() const override;
        std::unique_ptr<core::object> remove_object(
            const std::string& object_id
        ) override;
        uintmax_t space_used() const override;
    };

    class object : public core::object {
        repo::db::object entity;
    public:
        object(const repo::db::object&& entity);
        object(
            repo::db::object::db_t db,
            const repo::fs::fs_t& fs,
            const std::filesystem::path& path
        );

        const std::string& id() const override;
        const std::string& hash() const override;
        const std::string& mime_type() const override;
        uintmax_t size() const override;
    };

    class object_store : public core::object_store {
        repo::db::object_store entity;
        repo::fs::fs_t fs;
    public:
        object_store();
        std::unique_ptr<core::bucket> create_bucket(
            const std::string& name
        ) override;
        std::optional<std::unique_ptr<core::bucket>> fetch_bucket(
            const std::string& name
        ) override;
        std::vector<std::unique_ptr<core::bucket>> fetch_buckets() override;
        std::vector<std::unique_ptr<core::bucket>> fetch_buckets(
            const std::vector<std::string>& names
        ) override;
        std::unique_ptr<core::store_totals> get_store_totals() override;
        std::vector<std::unique_ptr<core::object>> prune() override;
    };
}
