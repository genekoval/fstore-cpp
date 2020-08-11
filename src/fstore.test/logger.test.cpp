#include <iostream>
#include <timber/timber>

namespace timber {
    auto handle_log(const log& l) noexcept -> void {
        std::cerr
            << '[' << l.log_level << "] "
            << l.stream.str()
            << std::endl;
    }
}
