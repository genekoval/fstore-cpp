#include <service.h>

#include <nova/ext/string.h>

namespace fs = std::filesystem;

namespace fstore::service {
    bucket_core::bucket_core(const repo::db::bucket&& entity) :
        entity(std::move(entity))
    {}

    std::unique_ptr<object> bucket_core::add_object(
        const std::filesystem::path& path
    ) {
        auto obj = std::make_unique<object_core>(path);
        entity.add(obj->id());
        return obj;
    }

    void bucket_core::drop() { entity.drop(); }

    std::string_view bucket_core::name() const { return entity.name(); }

    void bucket_core::name(std::string_view new_name) {
        entity.name(nova::ext::string::trim(std::string(new_name)));
    }

    int bucket_core::object_count() const { return entity.object_count(); }

    std::unique_ptr<object> bucket_core::remove_object(
        std::string_view object_id
    ) {
        return std::make_unique<object_core>(
            std::move(entity.remove(object_id))
        );
    }

    uintmax_t bucket_core::space_used() const { return entity.space_used(); }
}
