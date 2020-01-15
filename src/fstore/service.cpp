#include <repo.h>
#include <service.h>

#include <algorithm>
#include <cctype>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace fs = std::filesystem;
namespace object = repo::db::object;

using std::string;
using std::transform;

string sha256sum(string& sink, const fs::path& file) {
    using namespace CryptoPP;

    SHA256 hash;

    FileSource(
        file.c_str(),
        true,
        new HashFilter(
            hash,
            new HexEncoder(new StringSink(sink))
        )
    );

    transform(sink.begin(), sink.end(), sink.begin(),
        [](unsigned char c) -> unsigned char { return tolower(c); });

    return sink;
}

namespace service {
    void add_object(const string& bucket, const fs::path& path) {
        const fs::directory_entry file(path);

        string checksum;
        sha256sum(checksum, file.path());

        object::add(bucket, checksum, file.file_size());
    }
}
