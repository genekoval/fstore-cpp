#include <repo.h>

#include <commline/commands.h>
#include <iostream>

void commline::commands::bucket(const commline::cli& cli) {
    if (cli.args().empty())
        throw commline::cli_error("no bucket name given");

    const auto& name = cli.args().front();

    try {
        repo::db::bucket::create(name);
        std::cout << "created new bucket: " << name << std::endl;
    }
    catch (const repo::db::entity_exists_exception& ex) {
        throw commline::cli_error(ex.what());
    }
}
