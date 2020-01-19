#include <crypto.h>

#include <algorithm>
#include <cctype>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace fs = std::filesystem;

namespace fstore::crypto {
    void sha256sum(std::string& checksum, const fs::path& path) {
        using namespace CryptoPP;

        SHA256 hash;

        FileSource(
            path.c_str(),
            true,
            new HashFilter(
                hash,
                new HexEncoder(new StringSink(checksum))
            )
        );

        std::transform(checksum.begin(), checksum.end(), checksum.begin(),
            [](unsigned char c) -> unsigned char { return std::tolower(c); });
    }
}
