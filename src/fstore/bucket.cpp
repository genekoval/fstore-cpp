#include <commline/commands.h>
#include <fstore/service.h>
#include <iostream>
#include <nova/ext/string.h>
#include <utility>

namespace service = fstore::service;

namespace fstore {
    std::unique_ptr<service::bucket> fetch_bucket(
        const std::string& bucket_name
    ) {
        auto bucket = service::bucket_provider::fetch(bucket_name);

        if (!bucket) throw commline::cli_error(
            "bucket " QUOTE(nova::ext::string::trim(bucket_name))
            " does not exist"
        );

        return std::move(bucket.value());
    }
}

const std::string& get_name(const commline::cli& cli) {
    if (cli.args().empty())
        throw commline::cli_error("no bucket name given");

    return cli.args().front();
}

void commline::commands::create(const commline::cli& cli) {
    const auto& name = get_name(cli);

    try {
        const auto bucket = service::bucket_provider::create(name);
        std::cout << "created bucket: " << bucket->name() << std::endl;
    }
    catch (const fstore::core::fstore_error& ex) {
        throw commline::cli_error(ex.what());
    }
}

void commline::commands::remove(const commline::cli& cli) {
    const auto& name = get_name(cli);

    auto bucket = fstore::fetch_bucket(name);
    bucket->destroy();
    std::cout << "deleted bucket: " << bucket->name() << std::endl;
}

void commline::commands::rename(const commline::cli& cli) {
    if (cli.args().size() < 2)
        throw commline::cli_error("old and new bucket name required");

    const auto& old_name = cli.args()[0];
    const auto& new_name = cli.args()[1];

    auto bucket = fstore::fetch_bucket(old_name);

    const std::string old_bucket(bucket->name());
    bucket->name(new_name);

    std::cout
        << "bucket " QUOTE(old_bucket)
        << " renamed to: " << bucket->name()
        << std::endl;
}
