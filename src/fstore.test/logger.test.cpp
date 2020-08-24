#include <fstore/test.h>

#include <fstream>
#include <timber/timber>

const auto log_path =
    std::filesystem::temp_directory_path() / "fstore" / "fstore.test.log";
auto log_file = std::ofstream(log_path);

namespace timber {
    auto handle_log(const log& l) noexcept -> void {
        log_file
            << '[' << l.log_level << "] "
            << l.stream.str()
            << std::endl;
    }
}
