#include <cli.h>

#include <commline/commands.h>
#include <iostream>

void commline::commands::add(const commline::cli& cli) {
    if (!cli.options().selected("bucket"))
        throw commline::cli_error("bucket name required");

    if (cli.args().empty())
        throw commline::cli_error("no objects to add");

    auto bucket = fstore::fetch_bucket(
        cli.options().value("bucket").value()
    );

    for (const auto& file : cli.args()) {
        auto obj = bucket->add_object(file);
        std::cout << obj->id() << " " << file << std::endl;
    }
}
