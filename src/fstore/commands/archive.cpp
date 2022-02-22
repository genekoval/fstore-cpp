#include "commands.h"
#include "../db/db.h"
#include "../options/opts.h"

#include <fstore/cli.h>
#include <fstore/core/filesystem.h>

#include <fmt/format.h>

using namespace commline;

namespace {
    auto $archive(
        const app& app,
        std::string_view confpath,
        std::optional<std::string_view> file,
        timber::level log_level
    ) -> void {
        timber::reporting_level = log_level;

        const auto settings = fstore::conf::settings::load_file(confpath);
        const auto client = fstore::cli::data::client(settings);

        const auto archive = file.value_or(settings.archive.path);
        if (archive.empty()) throw std::runtime_error(
            "archive location not specified"
        );

        const auto dump_file = client.dump(settings.home);

        const auto fs = fstore::core::fs::filesystem(settings.home);

        auto options = std::vector<std::string_view>();
        for (const auto& option : settings.archive.options) {
            options.emplace_back(option);
        }

        options.push_back(dump_file);

        fs.sync(
            settings.archive.sync,
            options,
            archive
        );

        std::filesystem::remove(dump_file);
    }
}

namespace fstore::cli {
    auto archive(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Create a backup of the database and object files",
            options(
                opts::config(confpath),
                option<std::optional<std::string_view>>(
                    {"f", "file"},
                    "Send output to the specified file or directory",
                    "file"
                ),
                opts::log_level()
            ),
            arguments(),
            $archive
        );
    }
}
