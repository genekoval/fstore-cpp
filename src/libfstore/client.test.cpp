#include <fstore/client.h>
#include <fstore/test.h>

#include <fstream>
#include <gtest/gtest.h>
#include <uuid++/uuid.h>

using namespace std::literals;

const auto socket_file = std::filesystem::temp_directory_path() / "fstore.sock";
constexpr auto bucket_name = "test";

auto server_pid = pid_t();

namespace test_object {
    constexpr auto content = "Hello\n"sv;
    constexpr auto data = content.data();
    constexpr auto size = content.size();
    constexpr auto type = "text/plain"sv;
    constexpr auto hash =
        "66a045b452102c59d840ec097d59d9467e13a3f34f6494e539ffd32c1bb35f18"sv;
}

class ClientTest : public testing::Test {
protected:
    static auto SetUpTestSuite() -> void {
        fstore::test::drop_buckets();
        fstore::test::drop_objects();

        auto db = fstore::test::db();
        fstore::test::create_bucket(db, bucket_name);

        server_pid = fstore::test::start_server(socket_file);

        if (server_pid == 0) std::exit(EXIT_SUCCESS);
    }

    static auto TearDownTestSuite() -> void {
        fstore::test::stop_server(server_pid);
        fstore::test::drop_buckets();
        fstore::test::drop_objects();
    }

    fstore::object_store store;
    const fstore::core::bucket bucket_info;
    fstore::bucket bucket;

    ClientTest() :
        store(socket_file.string()),
        bucket_info(store.fetch_bucket(bucket_name)),
        bucket(bucket_info.id, store)
    {
        fstore::test::drop_objects();
    }

    auto add_object() -> fstore::object_meta {
        return bucket.add({}, test_object::size, [](auto&& part) {
            part.write(std::span(
                reinterpret_cast<const std::byte*>(test_object::data),
                test_object::size
            ));
        });
    }
};

TEST_F(ClientTest, FetchBucket) {
    ASSERT_FALSE(bucket_info.id.empty());
    ASSERT_FALSE(bucket_info.date_created.empty());
    ASSERT_EQ(bucket_name, bucket_info.name);
}

TEST_F(ClientTest, AddObject) {
    auto object = add_object();

    ASSERT_EQ(test_object::size, object.size);
    ASSERT_EQ(test_object::hash, object.hash);
    ASSERT_EQ(test_object::type, object.mime_type);
}

TEST_F(ClientTest, AddFile) {
    const auto directory = fstore::test::temp_directory();
    const auto filename = directory.path / "TestObject";

    {
        auto file = std::ofstream(filename);
        file << test_object::content;
    }

    auto object = bucket.add(filename.string());

    ASSERT_EQ(test_object::size, object.size);
    ASSERT_EQ(test_object::hash, object.hash);
    ASSERT_EQ(test_object::type, object.mime_type);
}

TEST_F(ClientTest, GetMetadata) {
    const auto object = add_object();
    const auto metadata = bucket.meta(object.id);

    ASSERT_EQ(object.id, metadata.id);
    ASSERT_EQ(object.hash, metadata.hash);
    ASSERT_EQ(object.size, metadata.size);
    ASSERT_EQ(object.mime_type, metadata.mime_type);
    ASSERT_EQ(object.date_added, metadata.date_added);
}

TEST_F(ClientTest, GetObjectContent) {
    const auto object = add_object();
    auto blob = bucket.get(object.id);

    ASSERT_EQ(test_object::size, blob.size());

    const auto text = std::string(
        reinterpret_cast<const char*>(blob.data()),
        blob.size()
    );

    ASSERT_EQ(test_object::content, text);
}

TEST_F(ClientTest, GetObjectNoObject) {
    auto uuid = UUID::uuid();
    uuid.generate();

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

TEST_F(ClientTest, RemoveObjects) {
    constexpr auto data = std::array {"foo"sv, "bar"sv, "baz"sv };

    auto size = 0ul;
    for (const auto& text : data) size += text.size();

    const auto expected = fstore::remove_result {
        .objects_removed = data.size(),
        .space_freed = size
    };

    auto objects = std::vector<std::string>();

    for (const auto& text : data) {
        const auto obj = bucket.add({}, text.size(), [&](auto&& part) {
            const auto* bytes = reinterpret_cast<const std::byte*>(text.data());
            part.write(std::span<const std::byte>(bytes, text.size()));
        });

        objects.push_back(obj.id);
    }

    const auto result = bucket.remove(objects);
    ASSERT_EQ(expected, result);
}
