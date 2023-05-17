#pragma once

#include <zipline/zipline>

namespace fstore {
    struct fstore_error : zipline::zipline_error {
        using zipline_error::zipline_error;
    };

    struct invalid_data : fstore_error {
        using fstore_error::fstore_error;
    };

    struct not_found : fstore_error {
        using fstore_error::fstore_error;
    };

    class unique_bucket_violation : public fstore_error {
        std::string _name;
    public:
        unique_bucket_violation(std::string_view name);

        auto encode(
            zipline::io::abstract_writer& writer
        ) const -> ext::task<> override;

        auto name() const -> std::string_view;
    };

    using error_list = zipline::error_list<
        invalid_data,
        not_found,
        unique_bucket_violation
    >;
}

namespace zipline {
    template <>
    struct decoder<fstore::unique_bucket_violation, io::abstract_reader> {
        static auto decode(
            io::abstract_reader& reader
        ) -> ext::task<fstore::unique_bucket_violation>;
    };
}
