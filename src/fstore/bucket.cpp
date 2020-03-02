#include <cli.h>

#include <commline/commands.h>
#include <iostream>
#include <nova/ext/string.h>
#include <utility>

namespace core = fstore::core;
namespace service = fstore::service;

using core::data_size;

namespace fstore {
    const bucket_table::row_t fstore::bucket_table::headers = {
        "Bucket", "Objects", "Space Used"
    };

    void bucket_table::get_data(row_t& entry, value_t&& bucket) {
        entry[0] = bucket->name();
        entry[1] = std::to_string(bucket->object_count());
        entry[2] = core::to_string(data_size::format(bucket->space_used()));
    }

    std::unique_ptr<service::bucket> fetch_bucket(
        const std::string& bucket_name
    ) {
        return std::move(fetch_bucket(
            bucket_name,
            std::move(service::object_store::get())
        ));
    }

    std::unique_ptr<service::bucket> fetch_bucket(
        const std::string& bucket_name,
        std::unique_ptr<service::object_store>&& object_store
    ) {
        auto bucket = object_store->fetch_bucket(bucket_name);

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
    const auto object_store = service::object_store::get();
    const auto& name = get_name(cli);

    try {
        const auto bucket = object_store->create_bucket(name);
        std::cout << "created bucket: " << bucket->name() << std::endl;
    }
    catch (const core::fstore_error& ex) {
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

void commline::commands::status(const commline::cli& cli) {
    const auto object_store = service::object_store::get();
    const auto show_all_buckets = cli.options().selected("all");
    fstore::bucket_table table;

    std::vector<std::unique_ptr<service::bucket>> buckets;

    if (show_all_buckets)
        buckets = object_store->fetch_buckets();
    else if (!cli.args().empty())
        buckets = object_store->fetch_buckets(cli.args());

    for (auto&& bucket : buckets) table.push_back(std::move(bucket));

    if (!table.empty())
        std::cout << std::endl << table << std::endl;

    if (cli.args().empty() || show_all_buckets) {
        auto totals = object_store->get_store_totals();

        std::cout
            << "buckets: " << totals->bucket_count() << std::endl
            << "objects: " << totals->object_count() << std::endl
            << "space used: " << data_size::format(totals->space_used())
            << std::endl;
    }
}
