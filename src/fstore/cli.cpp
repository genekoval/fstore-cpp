#include "cli.h"

#include <fstore/error.h>
#include <fstore/service.h>

#include <ext/data_size.h>
#include <ext/string.h>

namespace fstore {
    const bucket_table::row_t fstore::bucket_table::headers = {
        "Bucket", "Objects", "Space Used"
    };

    auto bucket_table::get_data(row_t& entry, value_t&& bucket) -> void {
        entry[0] = bucket->name();
        entry[1] = std::to_string(bucket->object_count());
        entry[2] = ext::to_string(ext::data_size::format(bucket->space_used()));
    }

    auto fetch_bucket(std::string_view name) -> std::unique_ptr<core::bucket> {
        return fetch_bucket(name, object_store());
    }

    auto fetch_bucket(
        std::string_view name,
        std::unique_ptr<core::object_store>&& object_store
    ) -> std::unique_ptr<core::bucket> {
        auto bucket = object_store->fetch_bucket(std::string(name));

        if (!bucket) throw fstore::fstore_error(
            "Bucket " QUOTE(ext::trim(std::string(name))) " does not exist."
        );

        return std::move(bucket.value());
    }

    auto object_store() -> std::unique_ptr<core::object_store> {
        return service::local_store(service::settings());
    }
}
