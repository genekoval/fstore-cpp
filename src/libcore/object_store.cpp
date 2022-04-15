#include <fstore/core/db/database.h>
#include <fstore/core/filesystem.h>
#include <fstore/core/object_store.h>

#include <uuid++/uuid.h>

namespace {
    auto generate_random_uuid() -> UUID::uuid {
        return UUID::generate();
    }
}

namespace fstore::core {
    object_store::object_store(
        db::database& database,
        fs::filesystem& filesystem
    ) :
        database(&database),
        filesystem(&filesystem),
        generate_uuid(generate_random_uuid)
    {}

    object_store::object_store(
        db::database& database,
        fs::filesystem& filesystem,
        uuid_generator generate_uuid
    ) :
        database(&database),
        filesystem(&filesystem),
        generate_uuid(generate_uuid)
    {}
}
