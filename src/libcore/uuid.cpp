#include <fstore/core.h>

namespace fstore::core {
    // 32 hex digits + 4 hyphens + 1 terminating null character
    constexpr int uuid_len = 37;

    // has_uuid {{{
    has_uuid::has_uuid(std::string_view uuid) : m_id(uuid) {}

    has_uuid::has_uuid(const core::uuid& uuid) : m_id(uuid) {}

    bool has_uuid::is_valid() const { return m_id.is_null(); }

    void has_uuid::nullify() { m_id.clear(); }

    std::string_view has_uuid::id() const { return m_id.to_string(); }

    void has_uuid::id(std::string_view new_id) { m_id = std::string(new_id); }
    /// }}}

    // uuid {{{
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
    /// }}}
}
