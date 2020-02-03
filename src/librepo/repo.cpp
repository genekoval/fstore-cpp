#include <database.h>

#include <fstore/core.h>
#include <fstore/repo.h>
#include <fstore/service.h>
#include <nova/ext/string.h>

using std::string;
using std::string_view;
using std::uintmax_t;

namespace fstore::repo::db {
    has_uuid::has_uuid(std::string_view uuid) : m_uuid(uuid) {}

    has_uuid::has_uuid(const core::uuid& uuid) : m_uuid(uuid) {}

    bool has_uuid::is_valid() const { return m_uuid.is_null(); }

    void has_uuid::nullify() { m_uuid.clear(); }

    std::string_view has_uuid::id() const { return m_uuid.to_string(); }

    object_entity::object_entity(
        const core::uuid& id,
        std::string_view hash,
        uintmax_t size
    ) :
        has_uuid(id),
        m_hash(hash),
        m_size(size)
    {}

    std::string_view object_entity::hash() const { return m_hash; }

    uintmax_t object_entity::size() const { return m_size; }

    bucket_entity::bucket_entity(const core::uuid& uuid, std::string_view name) :
        has_uuid(uuid),
        m_name(nova::ext::string::trim(std::string(name)))
    {
        pqxx::work transaction(connect());

        try {
            transaction.exec_params(
                "SELECT create_bucket($1, $2)",
                std::string(id()),
                m_name
            );
            transaction.commit();
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore::core::fstore_error(
                "cannot create bucket " QUOTE(m_name) ": bucket exists"
            );
        }
        catch (const pqxx::check_violation& ex) {
            throw fstore::core::fstore_error(
                "cannot create bucket: bucket name empty"
            );
        }
    }

    bucket_entity::bucket_entity(std::string_view name) :
        m_name(nova::ext::string::trim(std::string(name)))
    {
        pqxx::work transaction(connect());

        try {
            auto row = transaction.exec_params1(
                "SELECT id "
                "FROM bucket "
                "WHERE name = $1",
                m_name
            );

            m_uuid = row[0].as<std::string>();
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore::core::fstore_error(
                "failed to fetch bucket " QUOTE(m_name) ": "
                "bucket does not exist"
            );
        }
    }

    void bucket_entity::add_object(const service::object& obj) {
        pqxx::work transaction(connect());

        transaction.exec_params(
            "SELECT add_object($1, ("
                "SELECT create_object($2, $3, $4)"
            "))",
            std::string(id()),
            std::string(obj.id()),
            std::string(obj.hash()),
            obj.size()
        );

        transaction.commit();
    }

    void bucket_entity::destroy() {
        pqxx::work transaction(connect());

        transaction.exec_params(
            "SELECT delete_bucket($1)",
            std::string(id())
        );
        transaction.commit();
    }

    std::string_view bucket_entity::name() const { return m_name; }

    void bucket_entity::name(std::string_view name) {
        auto new_name = nova::ext::string::trim(std::string(name));

        pqxx::work transaction(connect());

        try {
            transaction.exec_params(
                "SELECT rename_bucket($1, $2)",
                std::string(id()),
                new_name
            );
            transaction.commit();
            m_name = new_name;
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore::core::fstore_error(
                "cannot rename bucket: bucket named "
                QUOTE(new_name) " already exists"
            );
        }
        catch (const pqxx::check_violation& ex) {
            throw fstore::core::fstore_error(
                "cannot rename bucket: name empty"
            );
        }
    }
}
