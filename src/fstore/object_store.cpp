#include <fstore/service.h>

#include <commline/commands.h>
#include <ext/data_size.h>
#include <iostream>

using ext::data_size;

void commline::commands::prune(const commline::cli& cli) {
    const auto object_store = fstore::service::local_store();
    const auto removed_objects = object_store->prune();

    if (removed_objects.empty()) {
        std::cout << "no objects to prune" << std::endl;
        return;
    }

    std::cout << "pruning object store..." << std::endl;
    std::cout << "---" << std::endl;

    uintmax_t freed_space = 0;
    for (const auto& object : removed_objects) {
        freed_space += object->size();
        std::cout
            << object->hash() << " "
            << data_size::format(object->size()) << std::endl;
    }

    std::cout << "---" << std::endl;
    std::cout
        << "deleted: " << removed_objects.size()
        << " object" << (removed_objects.size() == 1 ? "" : "s")
        << std::endl
        << "freeing: " << data_size::format(freed_space) << std::endl;
}
