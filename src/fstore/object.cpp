#include <cli.h>

#include <commline/commands.h>
#include <fstore/service.h>
#include <iostream>

using fstore::core::data_size;

void commline::commands::add(const commline::cli& cli) {
    if (!cli.options().selected("bucket"))
        throw commline::cli_error("bucket name required");

    if (cli.args().empty())
        throw commline::cli_error("no objects to add");

    auto bucket = fstore::fetch_bucket(
        cli.options().value("bucket").value()
    );

    for (const auto& file : cli.args()) {
        auto object = bucket->add_object(file);
        std::cout << object->id() << " " << file << std::endl;
    }
}

void commline::commands::object_remove(const commline::cli& cli) {
    if (!cli.options().selected("bucket"))
        throw commline::cli_error("bucket name required");

    if (cli.args().empty())
        throw commline::cli_error("no objects to remove");

    auto bucket = fstore::fetch_bucket(
        cli.options().value("bucket").value()
    );

    for (const auto& id : cli.args()) {
        try {
            auto object = bucket->remove_object(id);
            std::cout
                << "deleted: "
                << object->id() << " "
                << object->hash() << " "
                << data_size::format(object->size())
                << std::endl;
        }
        catch (const fstore::core::fstore_error& ex) {
            std::cout << "skipped: " << id << ": " << ex.what() << std::endl;
        }
    }
}
