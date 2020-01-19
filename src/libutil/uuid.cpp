#include <util.h>

#include <uuid/uuid.h>

namespace util::uuid {
    // 32 hex digits + 4 hyphens + 1 terminating null character
    constexpr int uuid_len = 37;

    uuid generate() {
        uuid_t uuid_obj;
        uuid_generate(uuid_obj);

        char uuid_str[uuid_len];
        uuid_unparse(uuid_obj, uuid_str);

        return uuid_str;
    }
}
