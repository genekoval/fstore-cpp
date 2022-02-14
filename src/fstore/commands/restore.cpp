#include "commands.h"
#include "../db/db.h"
#include "../options/opts.h"

#include <fstore/cli.h>
#include <fstore/repo/filesystem.h>

#include <fmt/format.h>

using namespace commline;

namespace {
    constexpr auto sync_options = std::array {
        "--archive",
        "--delete"
    };

    auto $restore(
        const app& app,
        std::string_view confpath,
        std::optional<std::string_view> user,
        timber::level log_level,
        std::optional<std::string> filename
    ) -> void {
        timber::reporting_level = log_level;

        auto settings = fstore::conf::settings::load_file(confpath);

        auto archive = filename.value_or(settings.archive.path);

        if (archive.empty()) throw std::runtime_error(
            "archive location not specified"
        );
        archive += "/";

        auto options = std::vector<std::string_view>();
        std::copy(
            sync_options.begin(),
            sync_options.end(),
            std::back_inserter(options)
        );
        for (const auto& option : settings.archive.options) {
            options.emplace_back(option);
        }
        options.push_back(archive);
        options.push_back(settings.home);

        const auto exit = ext::wait_exec(settings.archive.sync, options);

        if (exit.code != CLD_EXITED) throw std::runtime_error(fmt::format(
            "{} did not exit properly",
            settings.archive.sync
        ));

        if (exit.status != 0) throw std::runtime_error(fmt::format(
            "{} exited with code {}",
            settings.archive.sync,
            exit.status
        ));

        if (user) settings.database.connection.parameters["user"] = *user;
        const auto client = fstore::cli::data::client(settings);

        client.restore(settings.home);
    }
}

namespace fstore::cli {
    auto restore(
        std::string_view confpath
    ) -> std::unique_ptr<command_node> {
        return command(
            __FUNCTION__,
            "Create a backup of the database and object files",
            options(
                opts::config(confpath),
                option<std::optional<std::string_view>>(
                    {"u", "user"},
                    "User name to connect as",
                    "username"
                ),
                opts::log_level()
            ),
            arguments(
                optional<std::string>("filename")
            ),
            $restore
        );
    }
}
