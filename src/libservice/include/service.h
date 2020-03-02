#pragma once

#include <fstore/repo.h>
#include <fstore/service.h>

namespace fstore::service {
    class bucket_core : public core::has_uuid, public bucket {
        std::string m_name;
        int m_object_count;
        uintmax_t m_space_used;
    public:
        bucket_core(repo::db::bucket&& entity);
        bucket_core(std::string_view name);

        std::unique_ptr<object> add_object(
            const std::filesystem::path& file
        ) override;
        void destroy() override;
        std::string_view name() const override;
        void name(std::string_view new_name) override;
        int object_count() const override;
        std::unique_ptr<object> remove_object(
            std::string_view object_id
        ) override;
        uintmax_t space_used() const override;
    };

    class object_core : public core::has_uuid, public object {
        std::string m_hash;
        std::string m_mime_type;
        uintmax_t m_size;
    public:
        static void add_entities(
            std::vector<std::unique_ptr<object>>& objects,
            std::vector<repo::db::object>&& entities
        );

        object_core(const repo::db::object& entity);
        object_core(
            const bucket_core& bkt,
            const std::filesystem::path& path
        );

        std::string_view id() const override;
        std::string_view hash() const override;
        std::string_view mime_type() const override;
        uintmax_t size() const override;
    };

    class object_store_core : public object_store {
    public:
        std::unique_ptr<bucket> create_bucket(
            std::string_view name
        ) const override;
        std::optional<std::unique_ptr<bucket>> fetch_bucket(
            std::string_view name
        ) const override;
        std::vector<std::unique_ptr<bucket>> fetch_buckets() const override;
        std::vector<std::unique_ptr<bucket>> fetch_buckets(
            const std::vector<std::string>& names
        ) const override;
        std::unique_ptr<core::store_totals> get_store_totals() const override;
        std::vector<std::unique_ptr<object>> prune() const override;
    };
}
