#include <fstore/except.hpp>

namespace fstore {
    fstore_error::fstore_error() : runtime_error("fstore error") {}

    fstore_error::fstore_error(const std::string& what) : runtime_error(what) {}

    invalid_data::invalid_data(const std::string& what) : runtime_error(what) {}

    not_found::not_found(const std::string& what) : runtime_error(what) {}

    auto not_found::http_code() const noexcept -> int {
        return 404;
    }

    unique_bucket_violation::unique_bucket_violation(std::string_view name) :
        runtime_error(fmt::format(
            "Bucket with name '{}' already exists",
            name
        )),
        bucket_name(name)
    {}

    auto unique_bucket_violation::encode(
        zipline::io::abstract_writer& writer
    ) const -> ext::task<> {
        return zipline::encode(name(), writer);
    }

    auto unique_bucket_violation::name() const -> std::string_view {
        return bucket_name;
    }
}

namespace zipline {
    auto decoder<fstore::unique_bucket_violation, io::abstract_reader>::decode(
        io::abstract_reader& reader
    ) -> ext::task<fstore::unique_bucket_violation> {
        const auto name = co_await zipline::decode<std::string>(reader);
        co_return fstore::unique_bucket_violation(name);
    }
}
