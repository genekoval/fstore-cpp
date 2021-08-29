#include "db.h"

#include <fmt/format.h>
#include <timber/timber>

namespace fs = std::filesystem;

namespace {
    constexpr auto dump_file = "fstore.dump";
    constexpr auto stop_on_error = "--set=ON_ERROR_STOP=1";

    const auto sql_directory = fs::path(SQLDIR);

    const auto api_schema = (sql_directory / "fstore.sql").string();
    const auto data_schema = (sql_directory / "data.sql").string();
}

namespace fstore::cli::data {
    client::client(const conf::settings& settings) :
        client_program(settings.database.client),
        connection_string(settings.database.connection.str()),
        dump_program(settings.database.dump)
    {}

    auto client::dump(fs::path directory) const -> std::string {
        const auto file = (directory / dump_file).string();

        $(dump_program, "--format", "custom", "--file", file);
        DEBUG() << "Saved database dump to: " << file;

        return file;
    }

    auto client::exec(
        std::string_view program,
        std::span<const std::string_view> args
    ) const -> void {
        auto arguments = std::vector<std::string_view> {
            "--dbname", connection_string
        };

        std::copy(args.begin(), args.end(), std::back_inserter(arguments));

        if (timber::reporting_level >= timber::level::debug) {
            auto os = std::ostringstream();

            os << "EXEC " << program;
            for (const auto& arg : arguments) os << " " << arg;

            DEBUG() << os.str();
        }

        ext::exec(program, arguments);
    }

    auto client::exec(std::span<const std::string_view> args) const -> void {
        exec(client_program, args);
    }

    auto client::init() const -> void {
        $(client_program, stop_on_error, "--file", data_schema);
        migrate();
    }

    auto client::migrate() const -> void {
        $(client_program, stop_on_error, "--file", api_schema);
    }

    auto client::wait_exec(
        std::string_view program,
        std::span<const std::string_view> args
    ) const -> void {
        const auto parent = ext::process::fork();

        if (!parent) exec(program, args);

        const auto process = *parent;
        const auto exit = process.wait();

        if (exit.code == CLD_EXITED) {
            if (exit.status == 0) return;

            throw std::runtime_error(fmt::format(
                "{} exited with code {}",
                program,
                exit.status
            ));
        }

        if (exit.code == CLD_KILLED || exit.code == CLD_DUMPED) {
            throw std::runtime_error(fmt::format(
                "{} was killed by signal {}",
                program,
                exit.status
            ));
        }

        throw std::runtime_error(fmt::format(
            "{} did not succeed",
            program
        ));
    }
}
