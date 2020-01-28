#include <fstore/service.h>
#include <fstore/repo.h>

#include <commline/commands.h>
#include <iostream>

namespace service = fstore::service;

using std::string;

const string& get_name(const commline::cli& cli) {
    if (cli.args().empty())
        throw commline::cli_error("no bucket name given");

    return cli.args().front();
}

void commline::commands::create(const commline::cli& cli) {
    const auto& name = get_name(cli);

    const auto bucket = service::bucket_provider::create(name);
    if (!bucket) {
        throw commline::cli_error(
            "cannot create bucket ‘" + name + "‘: bucket already exists"
        );
    }

    std::cout << "created new bucket: " << bucket.value()->name() << std::endl;
}

void commline::commands::remove(const commline::cli& cli) {
    const auto& name = get_name(cli);

    service::bucket_provider::fetch(name).value()->destroy();
    std::cout << "deleted bucket: " << name << std::endl;
}
