#include <service.h>

#include <fstore/repo/filesystem.h>

#include <memory>

namespace fstore::service {
    object::object(const repo::db::object&& entity) :
        entity(std::move(entity))
    {}

    object::object(
        repo::db::object::db_t db,
        const repo::fs::fs_t& fs,
        const std::filesystem::path& path
    ) :
        entity(
            db,
            fs->hash(path),
            fs->mime_type(path),
            fs->size(path)
        )
    {
        fs->copy_object(path, id());
    }

    const std::string& object::id() const { return entity.id(); }

    const std::string& object::hash() const { return entity.hash(); }

    const std::string& object::mime_type() const {
        return entity.mime_type();
    }

    uintmax_t object::size() const { return entity.size(); }
}
