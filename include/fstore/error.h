#pragma once

#include <zipline/zipline>

namespace fstore {
    struct fstore_error : std::runtime_error {
        using runtime_error::runtime_error;
    };

    class unique_bucket_violation : public zipline::zipline_error {
        std::string _name;
    public:
        unique_bucket_violation(std::string_view name);

        auto encode(
            zipline::io::abstract_writer& writer
        ) const -> ext::task<> override;

        auto name() const -> std::string_view;
    };

    using error_list = zipline::error_list<
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
