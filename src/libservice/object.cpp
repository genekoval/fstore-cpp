#include <service.h>
#include <crypto.h>

namespace fstore::service {
    object_core::object_core(const fs::path& path) :
        m_hash(crypto::sha256sum(path)),
        m_size(fs::file_size(path))
    {}

    std::string_view object_core::id() const { return has_uuid::id(); }

    std::string_view object_core::hash() const { return m_hash; }

    uintmax_t object_core::size() const { return m_size; }
}
