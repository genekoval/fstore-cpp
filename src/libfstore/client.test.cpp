#include <fstore/client.h>
#include <fstore/test.h>

#include <fstream>
#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

const auto socket = std::filesystem::temp_directory_path() / "fstore.sock";
constexpr auto bucket_name = "test";

auto server_pid = pid_t();

class ClientTest : public testing::Test {
protected:
    static auto SetUpTestSuite() -> void {
        fstore::test::drop_buckets();
        fstore::test::drop_objects();

        auto db = fstore::test::db();
        fstore::test::create_bucket(db, bucket_name);

        server_pid = fstore::test::start_server(socket);

        if (server_pid == 0) std::exit(EXIT_SUCCESS);
    }

    static auto TearDownTestSuite() -> void {
        fstore::test::stop_server(server_pid);
        fstore::test::drop_buckets();
        fstore::test::drop_objects();
    }

    fstore::test::temp_directory objects;
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
    const auto filename = objects.path / "TestObject";
    constexpr auto content = "Hello\n";

    auto bucket = connect();

    {
        auto file = std::ofstream(filename);
        file << content;
    }

    auto object = bucket.add(filename.string());

    ASSERT_EQ(6, object.size);
    ASSERT_EQ(
        "66a045b452102c59d840ec097d59d9467e13a3f34f6494e539ffd32c1bb35f18",
        object.hash
    );
    ASSERT_EQ("text/plain", object.mime_type);
}

TEST_F(ClientTest, GetObject) {
    const auto filename = objects.path / "TestObject";
    constexpr auto content = "Hello\n";

    auto bucket = connect();

    {
        auto file = std::ofstream(filename);
        file << content;
    }

    auto object = bucket.add(filename.string());
    auto result = bucket.meta(object.id);

    ASSERT_EQ(object.id, result.id);
    ASSERT_EQ(object.hash, result.hash);
    ASSERT_EQ(object.size, result.size);
    ASSERT_EQ(object.mime_type, result.mime_type);
    ASSERT_EQ(object.date_added, result.date_added);
}

TEST_F(ClientTest, GetObjectContent) {
    const auto filename = objects.path / "TestObject";
    constexpr auto content = "Hello\n"sv;

    auto bucket = connect();

    {
        auto file = std::ofstream(filename);
        file << content;
    }

    const auto object = bucket.add(filename.string());

    const auto [type, data] = bucket.get(object.id);

    ASSERT_EQ("text/plain", type);
    ASSERT_EQ(content.size(), data.size());

    const auto text = std::string(
        reinterpret_cast<const char*>(data.data()),
        data.size()
    );

    ASSERT_EQ(content, text);
}

TEST_F(ClientTest, GetObjectNoObject) {
    auto uuid = UUID::uuid();
    uuid.generate();

    auto bucket = connect();

    try {
        bucket.meta(uuid.string());
        FAIL() << "Retrieved metadata for a non-existent object";
    }
    catch (const std::runtime_error& ex) {
        ASSERT_EQ(
            "bucket does not contain object"s,
            ex.what()
        );
    }
}
