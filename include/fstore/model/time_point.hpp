#pragma once

#include <chrono>

namespace fstore {
    using time_point = std::chrono::
        time_point<std::chrono::system_clock, std::chrono::milliseconds>;
}
