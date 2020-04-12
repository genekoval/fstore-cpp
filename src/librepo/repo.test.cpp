#include <repo.h>

constexpr auto test_db = "postgresql://fstore@localhost/fstore";

namespace fstore::test {
    auto test_store() -> repo::db::object_store {
        return repo::db::object_store(test_db);
    }
}
