target_sources(core PRIVATE
    connection.cpp
    database.cpp
    pool.cpp
)

add_subdirectory(model)

if(PROJECT_TESTING)
    target_sources(fstore.test PRIVATE
        bucket.test.cpp
        database.test.cpp
        object.test.cpp
    )

    add_subdirectory(test)
endif()
