#pragma once

#include <fstore/service.h>
#include <fstore/repo.h>

namespace fs = std::filesystem;

namespace fstore::service {
    class bucket_core : public bucket {
        repo::db::bucket_entity entity;
    public:
        bucket_core(const core::uuid& id, std::string_view name);
        bucket_core(std::string_view name);

        void add_object(const fs::path& path) override;
        void destroy() override;
        std::string_view name() const override;
        void name(std::string_view new_name) override;
    };

    class object_core : public object {
        repo::db::object_entity entity;
    public:
        object_core(const fs::path& path);

        core::uuid id() const override;
        std::string_view hash() const override;
        uintmax_t size() const override;
    };
}
