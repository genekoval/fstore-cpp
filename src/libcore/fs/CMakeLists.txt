target_sources(core PRIVATE
    filesystem.cpp
    part.cpp
)

add_subdirectory(crypto)

if(PROJECT_TESTING)
    target_sources(fstore.test PRIVATE
        filesystem.test.cpp
    )

    add_subdirectory(test)
endif()

