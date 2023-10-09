#pragma once

#include <internal/core/db/model/object.hpp>
#include <internal/core/fs/part.hpp>

#include <fstore/model/bucket.hpp>
#include <fstore/model/file.hpp>
#include <fstore/model/object.hpp>
#include <fstore/model/object_error.hpp>
#include <fstore/model/remove_result.hpp>
#include <fstore/model/store_totals.hpp>

#include <optional>
#include <span>

namespace fstore::core {
    namespace db {
        class database;
    }

    namespace fs {
        class filesystem;
    }

    struct check_progress {
        std::size_t success;
        std::size_t errors;
        std::size_t total;

        inline auto completed() const noexcept -> std::size_t {
            return success + errors;
        }
    };

    class object_store {
        using uuid_generator = auto (*)() -> UUID::uuid;

        db::database* database;
        fs::filesystem* filesystem;
        uuid_generator generate_uuid;

        auto check_object(const db::object& obj) noexcept -> std::string;

        auto check_object_task(
            const db::object obj,
            netcore::awaitable_thread_pool& workers,
            std::vector<object_error>& records,
            check_progress& progress,
            ext::counter& counter
        ) -> ext::detached_task;
    public:
        object_store(db::database& database, fs::filesystem& filesystem);

        object_store(
            db::database& database,
            fs::filesystem& filesystem,
            uuid_generator generate_uuid
        );

        auto check(int batch_size, int jobs, check_progress& progress)
            -> ext::task<>;

        auto commit_part(const UUID::uuid& bucket_id, const UUID::uuid& part_id)
            -> ext::task<object>;

        auto create_bucket(std::string_view name) -> ext::task<bucket>;

        auto fetch_bucket(std::string_view name) -> ext::task<bucket>;

        auto fetch_buckets() -> ext::task<std::vector<bucket>>;

        auto fetch_buckets(std::span<const std::string> names)
            -> ext::task<std::vector<bucket>>;

        auto fetch_store_totals() -> ext::task<store_totals>;

        auto get_errors() -> ext::task<std::vector<object_error>>;

        auto get_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<file>;

        auto get_object_metadata(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        auto get_part(std::optional<UUID::uuid> part_id) -> fs::part;

        auto prune() -> ext::task<std::vector<object>>;

        auto remove_bucket(const UUID::uuid& bucket_id) -> ext::task<>;

        auto remove_object(
            const UUID::uuid& bucket_id,
            const UUID::uuid& object_id
        ) -> ext::task<object>;

        auto remove_objects(
            const UUID::uuid& bucket_id,
            const std::vector<UUID::uuid>& objects
        ) -> ext::task<remove_result>;

        auto rename_bucket(
            const UUID::uuid& bucket_id,
            std::string_view bucket_name
        ) -> ext::task<>;
    };
}
