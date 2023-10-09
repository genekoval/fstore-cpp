#pragma once

#include <http/http>
#include <zipline/zipline>

namespace fstore {
    struct fstore_error : zipline::zipline_error {
        fstore_error();

        fstore_error(const std::string& what);
    };

    struct invalid_data : fstore_error {
        invalid_data(const std::string& what);

        template <typename... Args>
        invalid_data(
            fmt::format_string<Args...> format_string,
            Args&&... args
        ) :
            runtime_error(
                fmt::format(format_string, std::forward<Args>(args)...)
            ) {}
    };

    struct not_found : fstore_error, http::server::error {
        not_found(const std::string& what);

        template <typename... Args>
        not_found(fmt::format_string<Args...> format_string, Args&&... args) :
            runtime_error(
                fmt::format(format_string, std::forward<Args>(args)...)
            ) {}

        auto http_code() const noexcept -> int override;
    };

    class unique_bucket_violation : public fstore_error {
        std::string bucket_name;
    public:
        unique_bucket_violation(std::string_view name);

        auto encode(zipline::io::abstract_writer& writer) const
            -> ext::task<> override;

        auto name() const -> std::string_view;
    };

    using error_list =
        zipline::error_list<invalid_data, not_found, unique_bucket_violation>;
}

namespace zipline {
    template <>
    struct decoder<fstore::unique_bucket_violation, io::abstract_reader> {
        static auto decode(io::abstract_reader& reader)
            -> ext::task<fstore::unique_bucket_violation>;
    };
}
