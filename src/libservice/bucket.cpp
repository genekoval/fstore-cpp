#include <service.h>

#include <nova/ext/string.h>

namespace fs = std::filesystem;

namespace fstore::service {
    bucket::bucket(const repo::db::bucket&& entity) :
        entity(std::move(entity))
    {}

    std::unique_ptr<core::object> bucket::add_object(
        const std::filesystem::path& path
    ) {
        auto obj = std::make_unique<object>(path);
        entity.add(obj->id());
        return obj;
    }

    void bucket::drop() { entity.drop(); }

    std::string_view bucket::name() const { return entity.name(); }

    void bucket::name(std::string_view new_name) {
        entity.name(nova::ext::string::trim(std::string(new_name)));
    }

    int bucket::object_count() const { return entity.object_count(); }

    std::unique_ptr<core::object> bucket::remove_object(
        std::string_view object_id
    ) {
        return std::make_unique<object>(
            std::move(entity.remove(object_id))
        );
    }

    uintmax_t bucket::space_used() const { return entity.space_used(); }
}
