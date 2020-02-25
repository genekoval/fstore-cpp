#pragma once

#include <string> // std::string
#include <string_view> // std::string_view
#include <stdexcept> // std::runtime_error
#include <uuid/uuid.h>

namespace fstore::core {
    struct store_totals {
        const int bucket_count;
        const int object_count;
        const uintmax_t space_used;
    };

    class uuid {
        uuid_t m_value;
        std::string value_string;

        void set_string();
        void value(std::string_view value_str);
    public:
        uuid();
        uuid(const uuid& other);
        uuid(std::string_view value_str);

        uuid& operator=(std::string_view str);

        void clear();
        bool is_null() const;
        std::string_view to_string() const;
    };

    class has_uuid {
        core::uuid m_id;
    protected:
        has_uuid() = default;
        has_uuid(std::string_view uuid);
        has_uuid(const core::uuid& uuid);

        void id(std::string_view new_id);
        void nullify();
    public:
        std::string_view id() const;
        bool is_valid() const;
    };

    struct fstore_error : std::runtime_error {
        fstore_error(const std::string& what_arg);
    };
}
