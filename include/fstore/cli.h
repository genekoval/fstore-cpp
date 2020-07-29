#pragma once

#include <fstore/table.h>
#include <fstore/service.h>

#include <commline/commline>
#include <netcore/socket.h>
#include <timber/timber>

namespace commline {
    template <>
    auto parse(std::string_view argument) -> timber::level;
}

namespace fstore {
    std::unique_ptr<core::bucket> fetch_bucket(
        std::string_view name
    );

    std::unique_ptr<core::bucket> fetch_bucket(
        std::string_view name,
        std::unique_ptr<core::object_store>&& object_store
    );

    std::unique_ptr<core::object_store> object_store();

    auto handle_connection(netcore::socket&& client) -> void;

    class bucket_table :
        public table<bucket_table, std::unique_ptr<core::bucket>, 3>
    {
        friend class table<bucket_table, value_t, column_count>;

        static const row_t headers;

        static void get_data(row_t& entry, value_t&& bucket);
    };
}
