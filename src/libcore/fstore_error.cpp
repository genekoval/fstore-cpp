#include <fstore/core.h>

using std::runtime_error;
using std::string;

namespace fstore::core {
    fstore_error::fstore_error(const string& what_arg) :
        runtime_error(what_arg)
    {}
}
