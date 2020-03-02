#pragma once

#include <fstore/core.h>

#include <entix/entix.h>
#include <string_view>

namespace fstore::repo::db {
    namespace bucket_col {
        using namespace std::literals;

        extern inline constexpr auto id = "bucket_id"sv;
        extern inline constexpr auto name = "bucket_name"sv;
        extern inline constexpr auto object_count = "object_count"sv;
        extern inline constexpr auto space_used = "space_used"sv;
    }

    struct bucket : public entix::entity<
        entix::column<bucket_col::id, std::string>,
        entix::column<bucket_col::name, std::string>,
        entix::column<bucket_col::object_count, int>,
        entix::column<bucket_col::space_used, uintmax_t>
    >
    {
        enum {
            c_id,
            c_name,
            c_object_count,
            c_space_used
        };

        using entity::entity;

        std::string_view id() const { return ccol<c_id>().ref(); }

        std::string_view name() const { return ccol<c_name>().ref(); }

        int object_count() const { return ccol<c_object_count>().value(); }

        uintmax_t space_used() const { return ccol<c_space_used>().value(); }
    };

    namespace object_col {
        using namespace std::literals;

        extern inline constexpr auto id = "id"sv;
        extern inline constexpr auto hash = "hash"sv;
        extern inline constexpr auto mime_type = "mime_type"sv;
        extern inline constexpr auto size = "len"sv;
    }

    struct object : public entix::entity<
        entix::column<object_col::id, std::string>,
        entix::column<object_col::hash, std::string>,
        entix::column<object_col::mime_type, std::string>,
        entix::column<object_col::size, uintmax_t>
    >
    {
        enum {
            c_id,
            c_hash,
            c_mime_type,
            c_size
        };

        using entity::entity;

        std::string_view id() const { return ccol<c_id>().ref(); }

        std::string_view hash() const { return ccol<c_hash>().ref(); }

        std::string_view mime_type() const { return ccol<c_mime_type>().ref(); }

        uintmax_t size() const { return ccol<c_size>().value(); }
    };

    namespace store_totals_col {
        using namespace std::literals;

        extern inline constexpr auto bucket_count = "bucket_count"sv;
        extern inline constexpr auto object_count = "object_count"sv;
        extern inline constexpr auto space_used = "space_used"sv;
    }

    struct store_totals : public core::store_totals, public entix::entity<
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

        using entity::entity;

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
