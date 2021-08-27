#pragma once

#include <fstore/table.h>
#include <fstore/core/model.h>

#include <commline/commline>
#include <timber/timber>

namespace commline {
    template <>
    auto parse(std::string_view argument) -> timber::level;
}

namespace fstore::cli {
    class bucket_table : public table<bucket_table, core::bucket, 3> {
        friend class table<bucket_table, value_t, column_count>;

        static const row_t headers;

        static void get_data(row_t& entry, value_t&& bucket);
    };
}
