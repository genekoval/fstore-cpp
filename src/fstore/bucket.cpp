#include <repo.h>

#include <commline/commands.h>
#include <iostream>

using std::string;

const string& get_name(const commline::cli& cli) {
    if (cli.args().empty())
        throw commline::cli_error("no bucket name given");

    return cli.args().front();
}

void commline::commands::create(const commline::cli& cli) {
    const auto& name = get_name(cli);

    if (repo::db::bucket::create(name))
        std::cout << "created new bucket: " << name << std::endl;
    else
        throw commline::cli_error("bucket '" + name + "' already exists");
}

void commline::commands::remove(const commline::cli& cli) {
    const auto& name = get_name(cli);

    if (repo::db::bucket::remove(name))
        std::cout << "deleted bucket: " << name << std::endl;
    else
        throw commline::cli_error("bucket '" + name + "' does not exist");
}
