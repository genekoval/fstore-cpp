#include <internal/core/db/connection_wrapper.hpp>

namespace fstore::core::db {
    connection_wrapper::connection_wrapper(
        std::shared_ptr<connection>&& inner
    ) :
        inner(std::forward<std::shared_ptr<connection>>(inner))
    {}

    auto connection_wrapper::add_error(
        const UUID::uuid& object_id,
        std::string_view message
    ) -> ext::task<> {
        return inner->add_error(object_id, message);
    }

    auto connection_wrapper::add_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id,
        std::string_view hash,
        std::int64_t size,
        std::string_view type,
        std::string_view subtype
    ) -> ext::task<object> {
        return inner->add_object(
            bucket_id,
            object_id,
            hash,
            size,
            type,
            subtype
        );
    }

    auto connection_wrapper::clear_error(
        const UUID::uuid& object_id
    ) -> ext::task<> {
        return inner->clear_error(object_id);
    }

    auto connection_wrapper::create_bucket(
        std::string_view name
    ) -> ext::task<bucket> {
        return inner->create_bucket(name);
    }

    auto connection_wrapper::fetch_bucket(
        std::string_view name
    ) -> ext::task<bucket> {
        return inner->fetch_bucket(name);
    }

    auto connection_wrapper::fetch_buckets() ->
        ext::task<std::vector<bucket>>
    {
        return inner->fetch_buckets();
    }

    auto connection_wrapper::fetch_buckets(
        std::span<const std::string> names
    ) -> ext::task<std::vector<bucket>> {
        return inner->fetch_buckets(names);
    }

    auto connection_wrapper::fetch_store_totals() ->
        ext::task<store_totals>
    {
        return inner->fetch_store_totals();
    }

    auto connection_wrapper::get_errors() ->
        ext::task<std::vector<object_error>>
    {
        return inner->get_errors();
    }

    auto connection_wrapper::get_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        return inner->get_object(bucket_id, object_id);
    }

    auto connection_wrapper::get_objects(
        int batch_size
    ) -> ext::task<pg::portal<object>> {
        return inner->get_objects(batch_size);
    }

    auto connection_wrapper::remove_bucket(
        const UUID::uuid& id
    ) -> ext::task<> {
        return inner->remove_bucket(id);
    }

    auto connection_wrapper::remove_object(
        const UUID::uuid& bucket_id,
        const UUID::uuid& object_id
    ) -> ext::task<object> {
        return inner->remove_object(bucket_id, object_id);
    }

    auto connection_wrapper::remove_objects(
        const UUID::uuid& bucket_id,
        const std::vector<UUID::uuid>& objects
    ) -> ext::task<remove_result> {
        return inner->remove_objects(bucket_id, objects);
    }

    auto connection_wrapper::remove_orphan_objects() ->
        ext::task<std::vector<object>>
    {
        return inner->remove_orphan_objects();
    }

    auto connection_wrapper::rename_bucket(
        const UUID::uuid& id,
        std::string_view name
    ) -> ext::task<> {
        return inner->rename_bucket(id, name);
    }
}
