#include <service.h>

#include <fstore/repo/filesystem.h>

#include <memory>

namespace fstore::service {
    object::object(const repo::db::object&& entity) :
        entity(std::move(entity))
    {}

    object::object(const std::filesystem::path& path) : entity(
        repo::fs::hash(path),
        repo::fs::mime_type(path),
        repo::fs::size(path)
    ) {
        repo::fs::copy_to_store(path, id());
    }

    std::string_view object::id() const { return entity.id(); }

    std::string_view object::hash() const { return entity.hash(); }

    std::string_view object::mime_type() const {
        return entity.mime_type();
    }

    uintmax_t object::size() const { return entity.size(); }
}
