#include <repo.h>

#include <algorithm>
#include <cctype>
#include <commline/commands.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <filesystem>
#include <iostream>
#include <service.h>
#include <string>

namespace fs = std::filesystem;
namespace object = repo::db::object;

using std::cout;
using std::endl;
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

void commline::commands::add(const commline::cli& cli) {
    if (cli.args().size() < 2)
        throw commline::cli_error("bucket and filename missing");

    const auto& bucket = cli.args()[0];
    const fs::directory_entry file(cli.args()[1]);

    string checksum;
    sha256sum(checksum, file.path());

    object::add(bucket, checksum, file.file_size());

    cout << checksum << "  " << file.path() << endl;
}
