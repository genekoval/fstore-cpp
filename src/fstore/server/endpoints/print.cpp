#include "endpoints.h"

namespace fstore::server {
    auto $print(uintmax_t n) -> void {
        INFO() << "Print received: " << n;
    }

    auto print(protocol& proto) -> void {
        proto.reply($print);
    }
}
