#include <service.h>

#include <fstore/repo.h>

namespace fstore::service {
    object_core::object_core(const fs::path& path) :
        m_hash(repo::fs::hash(path)),
        m_size(repo::fs::size(path))
    {}

    void object_core::add_to_bucket(const bucket_core& bkt) {
        const auto id_of_hash = repo::db::add_object(
            bkt.id(),
            id(),
            hash(),
            size()
        );

        has_uuid::id(id_of_hash);
    }

    std::string_view object_core::id() const { return has_uuid::id(); }

    std::string_view object_core::hash() const { return m_hash; }

    uintmax_t object_core::size() const { return m_size; }
}
