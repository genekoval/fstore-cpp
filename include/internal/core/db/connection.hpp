#pragma once

#include "model/model.hpp"
#include "pool.hpp"

#include <internal/test.hpp>

#include <fstore/model/bucket.hpp>
#include <fstore/model/object.hpp>
#include <fstore/model/object_error.hpp>
#include <fstore/model/remove_result.hpp>
#include <fstore/model/store_totals.hpp>

namespace fstore::core::db {
    class connection {
        pool::item client;
    public:
        connection() = default;

        connection(pool::item&& client);

        connection(const connection&) = delete;

        connection(connection&&) = default;

        VIRTUAL_DESTRUCTOR(connection);

        auto operator=(const connection&) -> connection& = delete;

        auto operator=(connection&&) -> connection& = default;

        VIRTUAL auto add_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id,
            std::string_view hash,
            std::int64_t size,
            std::string_view type,
            std::string_view subtype
        ) -> ext::task<object>;

        VIRTUAL auto begin() -> ext::task<pg::transaction>;

        VIRTUAL auto create_bucket(std::string_view name) -> ext::task<bucket>;

        VIRTUAL auto fetch_bucket(std::string_view name) -> ext::task<bucket>;

        VIRTUAL auto fetch_buckets() -> ext::task<std::vector<bucket>>;

        VIRTUAL auto fetch_buckets(std::span<const std::string> names)
            -> ext::task<std::vector<bucket>>;

        VIRTUAL auto fetch_store_totals() -> ext::task<store_totals>;

        VIRTUAL auto get_errors() -> ext::task<std::vector<object_error>>;

        VIRTUAL auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        VIRTUAL auto get_objects(std::string_view portal, int batch_size)
            -> ext::task<pg::portal<object>>;

        VIRTUAL auto remove_bucket(const UUID::uuid& id) -> ext::task<>;

        VIRTUAL auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        VIRTUAL auto remove_objects(
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<remove_result>;

        VIRTUAL auto remove_orphan_objects() -> ext::task<std::vector<object>>;

        VIRTUAL auto rename_bucket(const UUID::uuid& id, std::string_view name)
            -> ext::task<>;

        VIRTUAL auto update_object_errors(std::span<const object_error> records)
            -> ext::task<>;
    };
}
