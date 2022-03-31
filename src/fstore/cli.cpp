#include <fstore/cli.h>

#include <ext/data_size.h>
#include <ext/string.h>

namespace commline {
    auto parser<timber::level>::parse(
        std::string_view argument
    ) -> timber::level {
        auto level = timber::parse_level(argument);

        if (!level) throw commline::cli_error(
            "unknown log level: {}",
            argument
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
        entry[2] = ext::data_size::format(bucket.space_used).str(2);
    }
}
