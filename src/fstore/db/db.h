#pragma once

#include <fstore/conf/settings.h>

namespace fstore::cli::data {
    class client {
        std::string client_program;
        std::string connection_string;
        std::string dump_program;

        auto exec(
            std::string_view program,
            std::span<const std::string_view> args
        ) const -> void;

        auto wait_exec(
            std::string_view program,
            std::span<const std::string_view> args
        ) const -> void;

        template <typename ...Args>
        auto $(std::string_view program, Args&&... args) const -> void {
            const auto arg_list = std::vector<std::string_view> {args...};
            wait_exec(program, arg_list);
        }
    public:
        client(const conf::settings& settings);

        auto dump(std::filesystem::path file) const -> std::string;

        auto exec(std::span<const std::string_view> args) const -> void;

        auto init() const -> void;

        auto migrate() const -> void;
    };
}
