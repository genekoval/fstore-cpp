#include "api.h"

#include <netcore/client.h>

namespace fstore {
    api::api(std::string_view path) :
        sock(netcore::connect(path)),
        client(sock)
    {}

    auto api::message(std::string_view msg) -> std::string {
        return client.send<std::string>(event::message, msg);
    }

    auto api::print(uintmax_t n) -> void {
        client.emit(event::print, n);
    }

    auto api::get_store_totals() -> store_totals {
        return client.send<store_totals>(event::get_store_totals);
    }
}
