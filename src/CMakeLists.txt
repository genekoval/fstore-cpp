add_subdirectory(libfstore)

if(FSTORE_BUILD_SERVER OR PROJECT_TESTING)
    add_subdirectory(fstore)
    add_subdirectory(libconf)
    add_subdirectory(libcore)
    add_subdirectory(libserver)
endif()

if(PROJECT_TESTING)
    add_subdirectory(fstore.test)
endif()
