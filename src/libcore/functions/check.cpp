#include "object_store.h"

#include <threadpool/threadpool>

namespace fstore::core {
    auto object_store::check_object(const object& obj) -> bool {
        try {
            const auto path = filesystem->object_path(obj.id);
            if (!std::filesystem::exists(path)) {
                database->add_error(obj.id, "file missing");
                return false;
            }

            const auto hash = filesystem->hash(path);
            if (hash != obj.hash) {
                database->add_error(obj.id, "hash mismatch");
                return false;
            }

            database->clear_error(obj.id);
            return true;
        }
        catch (const std::exception& ex) {
            database->add_error(obj.id, ex.what());
        }

        return false;
    }

    auto object_store::check(
        int jobs,
        check_progress& progress
    ) -> std::size_t {
        progress.total = database->fetch_store_totals().objects;
        std::atomic_ulong errors = 0;

        {
            auto workers = threadpool::pool(jobs);

            database->get_objects(100, [&](auto objects) {
                for (const auto& obj : objects) {
                    workers.run([this, obj, &errors, &progress]() {
                        const auto success = this->check_object(obj);
                        if (!success) errors += 1;
                        progress.completed += 1;
                    });
                }
            });
        }

        return errors;
    }
}
