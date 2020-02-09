#include <service.h>

#include <fstore/repo.h>

namespace fstore::service {
    object_core::object_core(const repo::db::object& data) :
        has_uuid(data.id),
        m_hash(data.hash),
        m_size(data.size)
    {}

    object_core::object_core(
        const bucket_core& bkt,
        const std::filesystem::path& path
    ) :
        m_hash(repo::fs::hash(path)),
        m_size(repo::fs::size(path))
    {
        has_uuid::id(repo::db::add_object(
            bkt.id(),
            id(),
            m_hash,
            m_size
        ));

        repo::fs::copy_to_store(path, id());
    }

    std::string_view object_core::id() const { return has_uuid::id(); }

    std::string_view object_core::hash() const { return m_hash; }

    uintmax_t object_core::size() const { return m_size; }
}
