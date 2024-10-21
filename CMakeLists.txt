cmake_minimum_required(VERSION 3.27)

include(cmake/modules.cmake)
include(GitVersion)

project(fstore
    VERSION ${GIT_VERSION_ABBREV}
    DESCRIPTION "Simple object storage"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

option(FSTORE_BUILD_SERVER "Build the fstore server executable" NO)
set(
    FSTORE_DEFAULT_CONFDIR
    "${PROJECT_BINARY_DIR}/config"
    CACHE PATH
    "Default directory for configuration files"
)

include(ProjectTesting)
include(packages.cmake)

add_library(fstore)
add_library(${PROJECT_NAME}::fstore ALIAS fstore)
target_sources(fstore PUBLIC FILE_SET HEADERS BASE_DIRS include)
target_link_libraries(fstore
    PUBLIC
        ext::ext
        http::http
        netcore::netcore
        nlohmann_json::nlohmann_json
        uuidcpp::uuid++
        zipline::zipline
    PRIVATE
        fmt::fmt
)
list(APPEND install_targets fstore)
list(APPEND install_dependencies
    ext
    http
    netcore
    nlohmann_json
    uuidcpp
    zipline
)

if(FSTORE_BUILD_SERVER OR PROJECT_TESTING)
    add_library(conf STATIC)
    target_include_directories(conf PUBLIC include)
    target_link_libraries(conf
        PUBLIC
            conftools::conftools
            cpp-postgres::pg++
    )

    add_library(core STATIC)
    target_compile_definitions(core PUBLIC
        $<$<BOOL:${PROJECT_TESTING}>:TEST>
    )
    target_include_directories(fstore
        PRIVATE
            ${CRYPTOPP_INCLUDE_DIRS}
    )
    target_link_libraries(core
        PUBLIC
            cpp-postgres::pg++
            ext::ext
            fstore::fstore
            netcore::netcore
            uuidcpp::uuid++
        PRIVATE
            ${CRYPTOPP_LIBRARIES}
            magix::magix
            timber::timber
    )

    add_library(server STATIC)
    target_link_libraries(server
        PUBLIC
            conftools::conftools
            cpp-postgres::pg++
            fstore::fstore
            http::http
        PRIVATE
            ext::ext
    )

    add_executable(fstored)
    set_target_properties(fstored PROPERTIES RUNTIME_OUTPUT_NAME fstore)
    target_compile_definitions(fstored PRIVATE
        NAME=\"fstore\"
        VERSION=\"${GIT_VERSION_FULL}\"
        DESCRIPTION=\"${PROJECT_DESCRIPTION}\"
        CONFDIR=\"${FSTORE_DEFAULT_CONFDIR}\"
    )
    target_link_libraries(fstored PRIVATE
        conf
        core
        commline::commline
        dbtools::dbtools
        dmon::dmon
        fstore::fstore
        server
    )

    if(FSTORE_BUILD_SERVER)
        list(APPEND install_targets fstored)
        list(APPEND install_dependencies
            commline
            conftools
            cpp-postgres
            dbtools
            dmon
            magix
            timber
        )
    endif()

    if(PROJECT_TESTING)
        add_executable(fstore.test)
        target_link_libraries(fstore.test PRIVATE
            conf
            core
            fstore::fstore
            GTest::gmock
            GTest::gtest
            server
        )
        add_test(
            NAME "Unit Tests"
            COMMAND fstore.test
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endif()
endif()

add_subdirectory(include)
add_subdirectory(src)

include(Sanitizers)
enable_asan()

include(EasyInstall)
easy_install(TARGETS ${install_targets} DEPENDENCIES ${install_dependencies})

if(PROJECT_IS_TOP_LEVEL)
    add_custom_target(docker
        docker build --progress=plain .
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        USES_TERMINAL
    )
    add_custom_target(run
        fstored start --log-level=trace
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        USES_TERMINAL
    )
endif()
