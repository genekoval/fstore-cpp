#include <service.h>
#include <crypto.h>

namespace fstore::service {
    object_core::object_core(const fs::path& path) : entity(
        fstore::core::generate_uuid(),
        crypto::sha256sum(path),
        fs::file_size(path)
    ) {}

    fstore::core::uuid object_core::id() const { return entity.id(); }

    std::string_view object_core::hash() const { return entity.hash(); }

    uintmax_t object_core::size() const { return entity.size(); }
}
