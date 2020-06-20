#include "cli.h"
#include "commands.h"

#include <ext/data_size.h>
#include <iostream>

auto $prune(const commline::app& app, const commline::argv& argv) -> void {
    const auto removed_objects = fstore::object_store()->prune();

    if (removed_objects.empty()) {
        std::cout << "No objects to prune." << std::endl;
        return;
    }

    std::cout << "Pruning object store..." << '\n' << "---" << '\n';

    uintmax_t freed_space = 0;
    for (const auto& object : removed_objects) {
        freed_space += object->size();
        std::cout
            << object->id() << " "
            << object->hash() << " "
            << ext::data_size::format(object->size())
            << '\n';
    }

    std::cout
        << "---" << '\n'
        << "Deleted: " << removed_objects.size()
        << " object" << (removed_objects.size() == 1 ? "" : "s") << '\n'
        << "Freeing: " << ext::data_size::format(freed_space) << std::endl;
}

namespace fstore::cli {
    auto prune() -> std::unique_ptr<commline::command_node> {
        return commline::command(
            "prune",
            "Remove any objects not referenced by a bucket.",
            $prune
        );
    }
}
