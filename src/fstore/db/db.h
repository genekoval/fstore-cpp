#pragma once

#include <fstore/conf/settings.h>

namespace fstore::cli::data {
    class client {
        std::string client_program;
        std::string connection_string;

        auto exec(
            std::string_view program,
            const std::vector<std::string_view>& args
        ) const -> void;

        auto wait_exec(
            std::string_view program,
            const std::vector<std::string_view>& args
        ) const -> void;
    public:
        client(const conf::settings& settings);

        auto exec(const std::vector<std::string_view>& args) const -> void;

        auto init() const -> void;

        auto migrate() const -> void;
    };
}
