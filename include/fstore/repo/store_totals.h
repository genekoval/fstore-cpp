#pragma once

#include <fstore/core.h>

#include <entix/entix.h>

namespace fstore::repo::db {
    namespace store_totals_col {
        using namespace std::literals;

        extern inline constexpr auto bucket_count = "bucket_count"sv;
        extern inline constexpr auto object_count = "object_count"sv;
        extern inline constexpr auto space_used = "space_used"sv;
    }

    struct store_totals : public core::store_totals, public entix::entity<
        store_totals,
        entix::column<store_totals_col::bucket_count, int>,
        entix::column<store_totals_col::object_count, int>,
        entix::column<store_totals_col::space_used, uintmax_t>
    >
    {
        enum {
            c_bucket_count,
            c_object_count,
            c_space_used
        };

        int bucket_count() const override {
            return ccol<c_bucket_count>().value();
        }

        int object_count() const override {
            return ccol<c_object_count>().value();
        }

        uintmax_t space_used() const override {
            return ccol<c_space_used>().value();
        }
    };
}
