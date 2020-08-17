#include <fstore/test.h>

constexpr auto directory = "fstore";

namespace fstore::test {
    auto temp_directory_path() -> std::filesystem::path {
        static auto dir = std::filesystem::temp_directory_path() / directory;
        std::filesystem::create_directory(dir);
        return dir;
    }
}
