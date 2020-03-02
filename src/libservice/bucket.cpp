#include <service.h>

#include <nova/ext/string.h>

namespace fs = std::filesystem;

namespace fstore::service {
    bucket_core::bucket_core(repo::db::bucket&& entity) :
        has_uuid(entity.id()),
        m_name(entity.name()),
        m_object_count(entity.object_count()),
        m_space_used(entity.space_used())
    {}

    bucket_core::bucket_core(std::string_view name) :
        m_name(nova::ext::string::trim(std::string(name)))
    {
        repo::db::create_bucket(id(), m_name);
    }

    std::unique_ptr<object> bucket_core::add_object(
        const std::filesystem::path& path
    ) {
        return std::make_unique<object_core>(*this, path);
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

    int bucket_core::object_count() const { return m_object_count; }

    std::unique_ptr<object> bucket_core::remove_object(
        std::string_view object_id
    ) {
        return std::make_unique<object_core>(
            repo::db::remove_object(id(), object_id)
        );
    }

    uintmax_t bucket_core::space_used() const { return m_space_used; }
}
