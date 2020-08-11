#include <fstore/cli.h>

#include <ext/data_size.h>
#include <ext/string.h>

namespace commline {
    template <>
    auto parse(std::string_view argument) -> timber::level {
        auto level = timber::parse_level(argument);

        if (!level) throw commline::cli_error(
            "unknown log level: " + std::string(argument)
        );

        return *level;
    }
}

namespace fstore::cli {
    const bucket_table::row_t bucket_table::headers = {
        "Bucket", "Objects", "Space Used"
    };

    auto bucket_table::get_data(row_t& entry, value_t&& bucket) -> void {
        entry[0] = bucket.name;
        entry[1] = std::to_string(bucket.size);
        entry[2] = ext::to_string(ext::data_size::format(bucket.space_used));
    }
}
