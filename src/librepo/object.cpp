#include <fstore/repo/object.h>

#include <uuid++/uuid.h>

using entix::query;

namespace fstore::repo::db {
    object::object(db_t db) : entity(db) {}

    object::object(
        db_t db,
        const std::string& hash,
        const std::string& mime_type,
        uintmax_t size
    ) :
        object(db)
    {
        col<c_hash>().set(hash);
        col<c_mime_type>().set(mime_type);
        col<c_size>().set(size);

        auto tx = pqxx::nontransaction(db->connect());

        auto uuid = UUID::uuid();
        uuid.generate();

        col<c_id>().set(tx.exec_params1(
            query{}
                .select("create_object($1, $2, $3, $4)")
            .str(),
            uuid.string(),
            hash,
            size,
            mime_type
        )[0].as<std::string>());
    }

    const std::string& object::id() const { return ccol<c_id>().ref(); }

    const std::string& object::hash() const { return ccol<c_hash>().ref(); }

    const std::string& object::mime_type() const {
        return ccol<c_mime_type>().ref();
    }

    uintmax_t object::size() const { return ccol<c_size>().value(); }
}
