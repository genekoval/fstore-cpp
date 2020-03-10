#include <service.h>

#include <fstore/repo/filesystem.h>

#include <memory>

namespace fstore::service {
    object_core::object_core(const repo::db::object&& entity) :
        entity(std::move(entity))
    {}

    object_core::object_core(const std::filesystem::path& path) : entity(
        repo::fs::hash(path),
        repo::fs::mime_type(path),
        repo::fs::size(path)
    ) {
        repo::fs::copy_to_store(path, id());
    }

    std::string_view object_core::id() const { return entity.id(); }

    std::string_view object_core::hash() const { return entity.hash(); }

    std::string_view object_core::mime_type() const {
        return entity.mime_type();
    }

    uintmax_t object_core::size() const { return entity.size(); }
}
