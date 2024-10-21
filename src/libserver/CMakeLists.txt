target_sources(server PRIVATE
    router.cpp
    server.cpp
)

add_subdirectory(http)
