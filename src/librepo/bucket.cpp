#include <fstore/error.h>
#include <fstore/repo/bucket.h>

using entix::query;

namespace fstore::repo::db {
    void bucket::add(const std::unique_ptr<core::object>& obj) {
        auto tx = pqxx::nontransaction(db->connect());

        tx.exec_params(
            query{}
                .select("add_object($1, $2)")
            .str(),
            id(),
            obj->id()
        );
    }

    void bucket::drop() {
        auto tx = pqxx::nontransaction(db->connect());

        tx.exec_params(
            query{}
                .select("delete_bucket($1)")
            .str(),
            id()
        );
    }

    const std::string& bucket::id() const { return ccol<c_id>().ref(); }

    const std::string& bucket::name() const { return ccol<c_name>().ref(); }

    void bucket::name(const std::string& new_name) {
        auto tx = pqxx::nontransaction(db->connect());

        try {
            tx.exec_params(
                query{}
                    .select("rename_bucket($1, $2)")
                .str(),
                id(),
                new_name
            );

            col<c_name>().set(new_name);
        }
        catch (const pqxx::unique_violation& ex) {
            throw fstore_error(
                "cannot rename bucket: bucket named "
                QUOTE(new_name) " already exists"
            );
        }
        catch (const pqxx::check_violation& ex) {
            throw fstore_error(
                "cannot rename bucket: name empty"
            );
        }
    }

    int bucket::object_count() const { return ccol<c_object_count>().value(); }

    object bucket::remove(const std::string& object_id) {
        auto tx = pqxx::nontransaction(db->connect());

        try {
            return create_entity<object>(tx.exec_params1(
                query{}
                    .select<object>()
                    .from("remove_object($1, $2)")
                .str(),
                id(),
                object_id
            ));
        }
        catch (const pqxx::unexpected_rows& ex) {
            throw fstore_error("bucket does not contain object");
        }
    }

    uintmax_t bucket::space_used() const {
        return ccol<c_space_used>().value();
    }
}
