#include <database.h>

#include <fstore/repo/object.h>

#include <uuid++/uuid.h>

using entix::query;

namespace fstore::repo::db {
    object::object(
        const std::string& hash,
        const std::string& mime_type,
        uintmax_t size
    ) {
        col<c_hash>().set(hash);
        col<c_mime_type>().set(mime_type);
        col<c_size>().set(size);

        auto tx = pqxx::nontransaction(connect());

        auto uuid = UUID::uuid{};
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

    std::string_view object::id() const { return ccol<c_id>().ref(); }

    std::string_view object::hash() const { return ccol<c_hash>().ref(); }

    std::string_view object::mime_type() const {
        return ccol<c_mime_type>().ref();
    }

    uintmax_t object::size() const { return ccol<c_size>().value(); }
}
