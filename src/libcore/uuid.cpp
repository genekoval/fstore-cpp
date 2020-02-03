#include <fstore/core.h>

namespace fstore::core {
    // 32 hex digits + 4 hyphens + 1 terminating null character
    constexpr int uuid_len = 37;

    uuid::uuid() {
        uuid_generate(m_value);
        set_string();
    }

    uuid::uuid(const uuid& other) {
        uuid_copy(m_value, other.m_value);
        set_string();
    }

    uuid::uuid(std::string_view value_str) {
        value(value_str);
    }

    uuid& uuid::operator=(std::string_view str) {
        value(str);
        return *this;
    }

    void uuid::clear() {
        uuid_clear(m_value);
        set_string();
    }

    bool uuid::is_null() const { return uuid_is_null(m_value) == 1; }

    void uuid::set_string() {
        char uuid_str[uuid_len];
        uuid_unparse(m_value, uuid_str);
        value_string = uuid_str;
    }

    std::string_view uuid::to_string() const { return value_string; }

    void uuid::value(std::string_view value_str) {
        uuid_parse(std::string(value_str).c_str(), m_value);
        set_string();
    }
}
