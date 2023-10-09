#include "crypto.hpp"

#include <algorithm>
#include <cctype>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

using CryptoPP::FileSource;
using CryptoPP::HashFilter;
using CryptoPP::HexEncoder;
using CryptoPP::SHA256;
using CryptoPP::StringSink;
using CryptoPP::StringSource;

namespace fstore::core::fs::crypto {
    auto sha256sum(std::span<const std::byte> buffer) -> std::string {
        auto hash = SHA256();
        auto sum = std::string();

        const auto* data =
            reinterpret_cast<const CryptoPP::byte*>(buffer.data());

        StringSource(
            data,
            buffer.size(),
            true,
            new HashFilter(hash, new HexEncoder(new StringSink(sum)))
        );

        std::transform(
            sum.begin(),
            sum.end(),
            sum.begin(),
            [](unsigned char c) -> unsigned char { return std::tolower(c); }
        );

        return sum;
    }

    auto sha256sum(const std::filesystem::path& path) -> std::string {
        auto hash = SHA256();
        auto sum = std::string();

        FileSource(
            path.c_str(),
            true,
            new HashFilter(hash, new HexEncoder(new StringSink(sum)))
        );

        std::transform(
            sum.begin(),
            sum.end(),
            sum.begin(),
            [](unsigned char c) -> unsigned char { return std::tolower(c); }
        );

        return sum;
    }
}
