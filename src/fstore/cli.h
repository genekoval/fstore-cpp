#pragma once

#include "table.h"

#include <fstore/service.h>

namespace fstore {
    std::unique_ptr<core::bucket> fetch_bucket(
        const std::string& bucket_name
    );

    std::unique_ptr<core::bucket> fetch_bucket(
        const std::string& bucket_name,
        std::unique_ptr<core::object_store>&& object_store
    );

    class bucket_table :
        public table<bucket_table, std::unique_ptr<core::bucket>, 3>
    {
        friend class table<bucket_table, value_t, column_count>;

        static const row_t headers;

        static void get_data(row_t& entry, value_t&& bucket);
    };
}
