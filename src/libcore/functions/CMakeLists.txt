if(PROJECT_TESTING)
    target_sources(fstore.test PRIVATE
        create_bucket.test.cpp
        object_store.test.cpp
        rename_bucket.test.cpp
    )
endif()
