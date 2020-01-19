#include <core.h>

#include <uuid/uuid.h>

namespace fstore::core {
    // 32 hex digits + 4 hyphens + 1 terminating null character
    constexpr int uuid_len = 37;

    uuid generate_uuid() {
        uuid_t uuid_data;
        uuid_generate(uuid_data);

        char uuid_str[uuid_len];
        uuid_unparse(uuid_data, uuid_str);

        return uuid_str;
    }
}
