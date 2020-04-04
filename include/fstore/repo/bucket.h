#pragma once

#include <fstore/repo/postgresql.h>
#include <fstore/repo/object.h>

#include <entix/entix>

namespace fstore::repo::db {
    namespace bucket_col {
        using namespace std::literals;

        extern inline constexpr auto id = "bucket_id"sv;
        extern inline constexpr auto name = "bucket_name"sv;
        extern inline constexpr auto object_count = "object_count"sv;
        extern inline constexpr auto space_used = "space_used"sv;
    }

    struct bucket : public entix::entity<
        bucket,
        postgresql,
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

        void add(const std::unique_ptr<core::object>& obj);

        void drop();

        std::string_view id() const;

        std::string_view name() const;

        void name(const std::string& new_name);

        int object_count() const;

        object remove(std::string_view object_id);

        uintmax_t space_used() const;
    };

}
