#pragma once

#include <fstore/core.h>

namespace fstore::core {
    uuid generate_uuid();

    class object_impl : public object {
        uuid _id;
    public:
        object_impl(uuid id);

        uuid id() const override;
    };

    class bucket_impl : public bucket {
        std::string _name;
    public:
        bucket_impl(std::string_view name);

        void add(const std::filesystem::path& path) override;
        std::string_view name() const override;
        // void name(std::string_view new_name) override;
        void remove() override;
    };
}
