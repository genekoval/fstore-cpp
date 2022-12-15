#include <fstore/error.h>

namespace fstore {
    unique_bucket_violation::unique_bucket_violation(std::string_view name) :
        fstore_error("bucket with name '{}' already exists", name),
        _name(name)
    {}

    auto unique_bucket_violation::encode(
        zipline::io::abstract_writer& writer
    ) const -> ext::task<> {
        return zipline::encode(name(), writer);
    }

    auto unique_bucket_violation::name() const -> std::string_view {
        return _name;
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
