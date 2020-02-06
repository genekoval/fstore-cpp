#include <crypto.h>

#include <algorithm>
#include <cctype>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

namespace fs = std::filesystem;

namespace fstore::crypto {
    std::string sha256sum(const fs::path& path) {
        using namespace CryptoPP;

        SHA256 hash;
        std::string sum;

        FileSource(
            path.c_str(),
            true,
            new HashFilter(
                hash,
                new HexEncoder(new StringSink(sum))
            )
        );

        std::transform(sum.begin(), sum.end(), sum.begin(),
            [](unsigned char c) -> unsigned char { return std::tolower(c); });

        return sum;
    }
}
