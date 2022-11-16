#include <fstore/client.h>

namespace fstore {
    bucket::bucket(object_store& store, const UUID::uuid& id) :
        store(&store),
        id(id)
    {}

    auto bucket::get(const UUID::uuid& object_id) -> ext::task<blob> {
        co_return co_await store->get_object(id, object_id);
    }

    auto bucket::get(
        const UUID::uuid& object_id,
        std::byte* buffer
    ) -> ext::task<> {
        co_await store->get_object(id, object_id, buffer);
    }

    auto bucket::get(
        const UUID::uuid& object_id,
        std::ostream& out
    ) -> ext::task<> {
        co_await store->get_object(id, object_id, out);
    }

    auto bucket::meta(const UUID::uuid& object_id) -> ext::task<object_meta> {
        co_return co_await store->get_object_metadata(id, object_id);
    }

    auto bucket::remove(const UUID::uuid& object_id) -> ext::task<object_meta> {
        co_return co_await store->remove_object(id, object_id);
    }

    auto bucket::remove(
        std::span<const UUID::uuid> objects
    ) -> ext::task<remove_result> {
        co_return co_await store->remove_objects(id, objects);
    }
}
