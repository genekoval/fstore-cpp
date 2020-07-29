#pragma once

#include <fstore/server/server.h>

#include <zipline/zipline>

namespace fstore {
    struct store_totals {
        const int bucket_count;
        const int object_count;
        const uintmax_t space_used;
    };

    class api {
        using protocol = zipline::protocol<netcore::socket>;

        enum class event : server::event_t {
            message,
            print,
            get_store_totals
        };

        const protocol::socket_type sock;
        zipline::client<protocol, event> client;
    public:
        api(std::string_view path);

        auto message(std::string_view msg) -> std::string;

        auto print(uintmax_t n) -> void;

        auto get_store_totals() -> store_totals;
    };
}

namespace zipline {
    template <typename Socket>
    struct transfer<Socket, fstore::store_totals> {
        static auto read(const Socket& sock) -> fstore::store_totals {
            using namespace fstore;

            return store_totals {
                .bucket_count = transfer<Socket, decltype(
                    store_totals::bucket_count
                )>::read(sock),
                .object_count = transfer<Socket, decltype(
                    store_totals::object_count
                )>::read(sock),
                .space_used = transfer<Socket, decltype(
                    store_totals::space_used
                )>::read(sock)
            };
        }
    };
}
