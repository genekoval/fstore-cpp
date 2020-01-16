#include <repo.h>
#include <service.h>

namespace crypto = util::crypto;
namespace fs = std::filesystem;
namespace object = repo::db::object;

using std::string;

namespace service {
    void add_object(const string& bucket, const fs::path& path) {
        const fs::directory_entry file(path);

        string checksum;
        crypto::sha256sum(checksum, file.path());

        object::add(bucket, checksum, file.file_size());
    }
}
