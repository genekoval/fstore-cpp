#include <util.h>

#include <algorithm>
#include <cctype>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

using std::string;

namespace util::crypto {
    void sha256sum(string& sink, const string& path) {
        using namespace CryptoPP;

        SHA256 hash;

        FileSource(
            path.c_str(),
            true,
            new HashFilter(
                hash,
                new HexEncoder(new StringSink(sink))
            )
        );

        std::transform(sink.begin(), sink.end(), sink.begin(),
            [](unsigned char c) -> unsigned char { return std::tolower(c); });
    }
}
