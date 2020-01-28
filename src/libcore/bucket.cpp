#include <core.h>
#include <crypto.h>

namespace fs = std::filesystem;

namespace fstore::core {
    // bucket_core {{{
    bucket_core::bucket_core(const uuid& id, std::string_view name) :
        entity(id, name)
    {}

    bucket_core::bucket_core(std::string_view name) : entity(name) {}

    void bucket_core::add_object(const std::filesystem::path& path) {
        object_core obj(path);
        entity.add_object(obj);
    }

    void bucket_core::destroy() { entity.destroy(); }

    std::string_view bucket_core::name() const { return entity.name(); }

    void bucket_core::name(std::string_view new_name) { entity.name(new_name); }
    // }}}

    // bucket_provider {{{
    std::optional<std::unique_ptr<bucket>> bucket_provider::create(
        std::string_view name
    ) {
        try {
            return std::unique_ptr<bucket>(new bucket_core(
                generate_uuid(),
                name
            ));
        }
        catch (const fstore_error& error) {
            return {};
        }
    }

    std::optional<std::unique_ptr<bucket>> bucket_provider::fetch(
        std::string_view name
    ) {
        return std::unique_ptr<bucket>(new bucket_core(name));
    }
    // }}}
}
