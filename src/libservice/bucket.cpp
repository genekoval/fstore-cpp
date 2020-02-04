#include <service.h>
#include <crypto.h>

#include <nova/ext/string.h>

namespace fs = std::filesystem;

namespace fstore::service {
    // bucket_core {{{
    bucket_core::bucket_core(
        const core::uuid& uuid,
        std::string_view name
    ) :
        has_uuid(uuid),
        m_name(nova::ext::string::trim(std::string(name)))
    {
        repo::db::create_bucket(id(), m_name);
    }

    bucket_core::bucket_core(std::string_view name) :
        m_name(nova::ext::string::trim(std::string(name)))
    {
        id(repo::db::fetch_bucket(m_name));
    }

    void bucket_core::add_object(const std::filesystem::path& path) {
        object_core obj(path);
        repo::db::add_object(
            id(),
            obj.id(),
            obj.hash(),
            obj.size()
        );
    }

    void bucket_core::destroy() {
        repo::db::delete_bucket(id());
        nullify();
    }

    std::string_view bucket_core::name() const { return m_name; }

    void bucket_core::name(std::string_view new_name) {
        const std::string processed_new_name =
            nova::ext::string::trim(std::string(new_name));

        repo::db::rename_bucket(id(), processed_new_name);
        m_name = processed_new_name;
    }
    // }}}

    // bucket_provider {{{
    std::unique_ptr<bucket> bucket_provider::create(
        std::string_view name
    ) {
        return std::unique_ptr<bucket>(new bucket_core(
            core::uuid{},
            name
        ));
    }

    std::optional<std::unique_ptr<bucket>> bucket_provider::fetch(
        std::string_view name
    ) {
        try {
            return std::unique_ptr<bucket>(new bucket_core(name));
        }
        catch (const fstore::core::fstore_error& ex) {
            return {};
        }
    }
    // }}}
}
