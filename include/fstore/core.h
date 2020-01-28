#pragma once

#include <string> // std::string
#include <stdexcept> // std::runtime_error

namespace fstore::core {
    using uuid = std::string;

    uuid generate_uuid();

    struct fstore_error : std::runtime_error {
        fstore_error(const std::string& what_arg);
    };
}
