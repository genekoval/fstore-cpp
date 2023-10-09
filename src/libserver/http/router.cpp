#include <internal/server/http/server.hpp>

namespace fstore::server::http {
    using ::http::server::del;
    using ::http::server::extractor::header;
    using ::http::server::extractor::path;
    using ::http::server::extractor::stream;
    using ::http::server::file;
    using ::http::server::get;

    auto router(const server_info& info, core::object_store& store)
        -> ::http::server::router {
        auto paths = ::http::server::path();

        paths.insert("/", get([&info]() { return info; }));

        paths.insert(
            "/bucket/:name",
            get([&store](path<"name"> name) -> ext::task<bucket> {
                co_return co_await store.fetch_bucket(name);
            })
        );

        paths.insert(
            "/object/:bucket/:object",
            get([&store](
                    path<"bucket", UUID::uuid> bucket_id,
                    path<"object", UUID::uuid> object_id
                ) -> ext::task<object> {
                co_return co_await store.get_object_metadata(
                    bucket_id,
                    object_id
                );
            })
                .del(
                    [&store](
                        path<"bucket", UUID::uuid> bucket_id,
                        path<"object", UUID::uuid> object_id
                    ) -> ext::task<object> {
                        co_return co_await store.remove_object(
                            bucket_id,
                            object_id
                        );
                    }
                )
        );

        paths.insert(
            "/object/:bucket",
            del([&store](
                    path<"bucket", UUID::uuid> bucket_id,
                    std::string objects
                ) -> ext::task<remove_result> {
                auto ids = std::vector<UUID::uuid>();

                for (const auto& line : ext::string_range(objects, "\n")) {
                    const auto trimmed = ext::trim(line);
                    if (!trimmed.empty()) ids.emplace_back(trimmed);
                }

                co_return co_await store.remove_objects(bucket_id, ids);
            })
                .post(
                    [&store](
                        path<"bucket", UUID::uuid> bucket_id,
                        header<"content-length", std::size_t> content_length,
                        stream stream
                    ) -> ext::task<object> {
                        auto part_id = UUID::uuid();

                        {
                            auto part = store.get_part({});
                            part_id = part.id;

                            std::size_t written = 0;

                            while (written < content_length) {
                                const auto chunk = co_await stream.read();

                                if (chunk.empty()) continue;

                                part.write(chunk);
                                written += chunk.size();
                            }
                        }

                        co_return co_await store.commit_part(
                            bucket_id,
                            part_id
                        );
                    }
                )
        );

        paths.insert(
            "/data/:bucket/:object",
            get([&store](
                    path<"bucket", UUID::uuid> bucket_id,
                    path<"object", UUID::uuid> object_id
                ) -> ext::task<file> {
                auto obj = co_await store.get_object(bucket_id, object_id);

                co_return file {
                    .fd = std::move(obj.fd),
                    .size = obj.metadata.size,
                    .content_type = obj.metadata.mime_type()};
            })
        );

        TIMBER_DEBUG("Routes:\n{}", paths.to_string());

        return paths;
    }
}
