#include <service.h>
#include <crypto.h>

namespace fstore::service {
    object_core::object_core(const fs::path& path) : entity(
        core::uuid(),
        crypto::sha256sum(path),
        fs::file_size(path)
    ) {}

    std::string_view object_core::id() const { return entity.id(); }

    std::string_view object_core::hash() const { return entity.hash(); }

    uintmax_t object_core::size() const { return entity.size(); }
}
