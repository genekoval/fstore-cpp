#include <service.h>

#include <nova/ext/string.h>

namespace fs = std::filesystem;

namespace fstore::service {
    bucket::bucket(
        const repo::db::bucket&& entity,
        repo::fs::fs_t fs
    ) :
        entity(std::move(entity)),
        fs(fs)
    {}

    std::unique_ptr<core::object> bucket::add_object(
        const std::filesystem::path& path
    ) {
        std::unique_ptr<core::object> obj = std::make_unique<object>(
            entity.connection(),
            fs,
            path
        );
        entity.add(obj);
        return obj;
    }

    void bucket::drop() { entity.drop(); }

    const std::string& bucket::name() const { return entity.name(); }

    void bucket::name(const std::string& new_name) {
        entity.name(nova::ext::string::trim(std::string(new_name)));
    }

    int bucket::object_count() const { return entity.object_count(); }

    std::unique_ptr<core::object> bucket::remove_object(
        const std::string& object_id
    ) {
        return std::make_unique<object>(
            std::move(entity.remove(object_id))
        );
    }

    uintmax_t bucket::space_used() const { return entity.space_used(); }
}
