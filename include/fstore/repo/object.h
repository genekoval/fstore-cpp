#pragma once

#include <fstore/core.h>
#include <fstore/repo/postgresql.h>

#include <entix/entix>

namespace fstore::repo::db {
    namespace object_col {
        using namespace std::literals;

        extern inline constexpr auto id = "id"sv;
        extern inline constexpr auto hash = "hash"sv;
        extern inline constexpr auto mime_type = "mime_type"sv;
        extern inline constexpr auto size = "len"sv;
    }

    struct object : public core::object, public entix::entity<
        object,
        postgresql,
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

        object(db_t db);

        object(
            db_t db,
            const std::string& hash,
            const std::string& mime_type,
            uintmax_t size
        );

        std::string_view id() const override;

        std::string_view hash() const override;

        std::string_view mime_type() const override ;

        uintmax_t size() const override;
    };
}
