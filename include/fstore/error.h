#pragma once

#include <stdexcept> // std::runtime_error

namespace fstore {
    struct fstore_error : std::runtime_error {
        using runtime_error::runtime_error;
    };
}
