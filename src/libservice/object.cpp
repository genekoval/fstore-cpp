#include <service.h>

#include <memory>

namespace fstore::service {
    object_core::object_core(const repo::db::object& entity) :
        has_uuid(entity.id()),
        m_hash(entity.hash()),
        m_mime_type(entity.mime_type()),
        m_size(entity.size())
    {}

    object_core::object_core(
        const bucket_core& bkt,
        const std::filesystem::path& path
    ) :
        m_hash(repo::fs::hash(path)),
        m_mime_type(repo::fs::mime_type(path)),
        m_size(repo::fs::size(path))
    {
        has_uuid::id(repo::db::add_object(
            bkt.id(),
            id(),
            m_hash,
            m_size,
            m_mime_type
        ));

        repo::fs::copy_to_store(path, id());
    }

    void object_core::add_entities(
        std::vector<std::unique_ptr<object>>& objects,
        std::vector<repo::db::object>&& entities
    ) {
        for (const auto& entity : entities)
            objects.push_back(std::make_unique<object_core>(entity));
    }

    std::string_view object_core::id() const { return has_uuid::id(); }

    std::string_view object_core::hash() const { return m_hash; }

    std::string_view object_core::mime_type() const { return m_mime_type; }

    uintmax_t object_core::size() const { return m_size; }
}
