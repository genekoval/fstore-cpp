#include <fstore/core.h>
#include <fstore/repo.h>

#include <commline/commands.h>
#include <iostream>

namespace core = fstore::core;

using std::string;

const string& get_name(const commline::cli& cli) {
    if (cli.args().empty())
        throw commline::cli_error("no bucket name given");

    return cli.args().front();
}

void commline::commands::create(const commline::cli& cli) {
    const auto& name = get_name(cli);

    auto bucket = core::bucket::create(name);
    std::cout << "created new bucket: " << bucket->name() << std::endl;
}

void commline::commands::remove(const commline::cli& cli) {
    const auto& name = get_name(cli);

    fstore::repo::db::bucket::remove(name);
    std::cout << "deleted bucket: " << name << std::endl;
}
