#include <database.h>

#include <fstore/core.h>
#include <fstore/repo.h>
#include <fstore/service.h>

using std::string;
using std::string_view;
using std::uintmax_t;

namespace fstore::repo::db {
    using core::uuid;

    has_id::has_id(const uuid& id) : m_id(id) {}

    uuid has_id::id() const { return m_id; }

    object_entity::object_entity(
        const uuid& id,
        std::string_view hash,
        uintmax_t size
    ) :
        has_id(id),
        m_hash(hash),
        m_size(size)
    {}

    std::string_view object_entity::hash() const { return m_hash; }

    uintmax_t object_entity::size() const { return m_size; }

    bucket_entity::bucket_entity(const uuid& id, std::string_view name) :
        has_id(id),
        m_name(name)
    {
        pqxx::work transaction(connect());

        try {
            transaction.exec_params(
                "SELECT create_bucket($1, $2)",
                id,
                std::string(name)
            );
            transaction.commit();
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore::core::fstore_error(
                "failed to create bucket ‘" + std::string(name) + "‘: "
                "bucket exists"
            );
        }
    }

    bucket_entity::bucket_entity(std::string_view name) : m_name(name) {
        pqxx::work transaction(connect());

        auto row = transaction.exec_params1(
            "SELECT id "
            "FROM bucket "
            "WHERE name = $1",
            std::string(name)
        );

        m_id = row[0].as<uuid>();
    }

    void bucket_entity::add_object(const service::object& obj) {
        pqxx::work transaction(connect());

        transaction.exec_params(
            "SELECT add_object($1, ("
                "SELECT create_object($2, $3, $4)"
            "))",
            m_id,
            obj.id(),
            std::string(obj.hash()),
            obj.size()
        );

        transaction.commit();
    }

    void bucket_entity::destroy() {
        pqxx::work transaction(connect());

        transaction.exec_params(
            "SELECT delete_bucket($1)",
            m_id
        );
        transaction.commit();
    }

    std::string_view bucket_entity::name() const { return m_name; }

    void bucket_entity::name(std::string_view name) {
        pqxx::work transaction(connect());

        transaction.exec_params(
            "SELECT rename_bucket($1, $2)",
            m_id,
            std::string(name)
        );

        transaction.commit();
    }
}
