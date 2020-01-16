#pragma once

#include <string>

namespace util {
    namespace crypto {
        void sha256sum(
            std::string& sink,
            const std::string& path
        );
    }

    namespace uuid {
        using uuid = std::string;

        uuid generate();
    }
}
