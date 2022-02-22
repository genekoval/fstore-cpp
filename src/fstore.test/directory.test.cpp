#include <fstore/test.h>

#include <memory>
#include <stdlib.h>

namespace {
    constexpr auto directory_template = "fstore.test.XXXXXX";

    auto make_temp_directory() -> std::filesystem::path {
        const auto path =
            std::filesystem::temp_directory_path() / directory_template;
        const auto string = path.string();
        const auto size = string.size();

        auto directory = std::make_unique<char[]>(size);
        string.copy(directory.get(), size);

        return std::filesystem::path(mkdtemp(directory.get()));
    }
}

namespace fstore::test {
    temp_directory::temp_directory() : path(make_temp_directory()) {}

    temp_directory::~temp_directory() {
        std::filesystem::remove_all(path);
    }
}
