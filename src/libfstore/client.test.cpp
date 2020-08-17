#include <fstore/client.h>
#include <fstore/test.h>

#include <fstream>
#include <gtest/gtest.h>

const auto socket = fstore::test::temp_directory_path() / "fstore.sock";
constexpr auto bucket_name = "test";

pid_t server_pid;

class ClientTest : public testing::Test {
protected:
    static auto SetUpTestSuite() -> void {
        auto db = fstore::test::db();
        fstore::test::create_bucket(db, bucket_name);

        server_pid = fstore::test::start_server(socket.string());
    }

    static auto TearDownTestSuite() -> void {
        fstore::test::stop_server(server_pid);
        fstore::test::drop_buckets();
        fstore::test::drop_objects();
    }

    fstore::object_store store;

    ClientTest() : store(socket.string()) {}

    auto connect() -> fstore::bucket {
        return fstore::bucket(
            store.fetch_bucket(bucket_name).id,
            store
        );
    }
};

TEST_F(ClientTest, FetchBucket) {
    auto info = store.fetch_bucket(bucket_name);

    ASSERT_FALSE(info.id.empty());
    ASSERT_FALSE(info.date_created.empty());
    ASSERT_EQ(bucket_name, info.name);
}

TEST_F(ClientTest, AddObject) {
    constexpr auto filename = "/tmp/fstore/TestObject";
    constexpr auto content = "Hello\n";

    auto bucket = connect();

    {
        auto file = std::ofstream(filename);
        file << content;
    }

    auto object = bucket.add(filename);

    ASSERT_EQ(6, object.size);
    ASSERT_EQ(
        "66a045b452102c59d840ec097d59d9467e13a3f34f6494e539ffd32c1bb35f18",
        object.hash
    );
    ASSERT_EQ("text/plain", object.mime_type);
}
