#pragma once

#include <string> // std::string
#include <string_view> // std::string_view
#include <stdexcept> // std::runtime_error
#include <uuid/uuid.h>

namespace fstore::core {
    class uuid {
        uuid_t m_value;
        std::string value_string;

        void set_string();
        void value(std::string_view value_str);
    public:
        uuid();
        uuid(const uuid& other);
        uuid(std::string_view value_str);

        uuid& operator=(const std::string& str);

        void clear();
        bool is_null() const;
        std::string_view to_string() const;
    };

    struct fstore_error : std::runtime_error {
        fstore_error(const std::string& what_arg);
    };
}
