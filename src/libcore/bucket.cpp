#include <core.h>
#include <crypto.h>

#include <fstore/repo.h>

namespace fs = std::filesystem;

namespace fstore::core {
    namespace db = repo::db;

    std::unique_ptr<bucket> bucket::create(std::string_view name) {
        db::bucket::create(generate_uuid(), std::string(name));
        return std::unique_ptr<bucket>(new bucket_impl(name));
    }

    bucket_impl::bucket_impl(std::string_view name) : _name(name) {}

    void bucket_impl::add(const fs::path& path) {
        const fs::directory_entry file(path);

        std::string checksum;
        crypto::sha256sum(checksum, file.path());

        db::object::add(_name, generate_uuid(), checksum, file.file_size());
    }

    std::string_view bucket_impl::name() const { return _name; }

    void bucket_impl::remove() { db::bucket::remove(_name); }
}
