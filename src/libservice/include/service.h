#pragma once

#include <fstore/repo.h>
#include <fstore/service.h>

namespace fstore::service {
    class bucket_core : public core::has_uuid, public bucket {
        std::string m_name;
    public:
        bucket_core(const core::uuid& uuid, std::string_view name);
        bucket_core(std::string_view name);

        std::unique_ptr<object> add_object(
            const std::filesystem::path& file
        ) override;
        void destroy() override;
        std::string_view name() const override;
        void name(std::string_view new_name) override;
    };

    class object_core : public core::has_uuid, public object {
        std::string m_hash;
        uintmax_t m_size;
    public:
        object_core(
            const bucket_core& bkt,
            const std::filesystem::path& path
        );

        std::string_view id() const override;
        std::string_view hash() const override;
        uintmax_t size() const override;
    };
}
