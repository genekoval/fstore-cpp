#include <fstore/test.h>

#include <memory>
#include <stdlib.h>

namespace fstore::test {
    constexpr auto directory_template = "fstore.test.XXXXXX";

    temp_directory::temp_directory() : path([]() -> std::filesystem::path {
        auto path = std::filesystem::temp_directory_path()/directory_template;
        auto string = path.string();
        auto size = string.size();

        auto directory = std::make_unique<char[]>(size);
        string.copy(directory.get(), size);

        auto result = std::filesystem::path(mkdtemp(directory.get()));
        return result;
    }()) {}

    temp_directory::~temp_directory() {
        std::filesystem::remove_all(path);
    }
}
