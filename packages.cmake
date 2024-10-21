include(FetchContent)

cmake_policy(PUSH)
cmake_policy(SET CMP0150 NEW)

FetchContent_Declare(commline
    GIT_REPOSITORY ../commline.git
    GIT_TAG        aea2cd146b654e28db102ba7c6b0489bfbc1ef57 # 0.4.0
)

FetchContent_Declare(conftools
    GIT_REPOSITORY ../conftools.git
    GIT_TAG        2dd61814544d7ec9df2958d1936da557dfc60b11 # 0.2.0
)

FetchContent_Declare(cpp-postgres
    GIT_REPOSITORY ../cpp-postgres.git
    GIT_TAG        4edc6eeb2af2fdd0f148cd814f2481546dc49689 # 0.4.0
)

FetchContent_Declare(dbtools
    GIT_REPOSITORY ../dbtools.git
    GIT_TAG        01241a0651802e2582ea4d2bf1892b277d8c3a9f # 0.4.0
)

FetchContent_Declare(dmon
    GIT_REPOSITORY ../dmon.git
    GIT_TAG        c13662cb1bc1d69f4e1afa18c84f00b4d640042a # 0.3.0
)

FetchContent_Declare(ext
    GIT_REPOSITORY ../libext.git
    GIT_TAG        76265c1325028676ae3219505bb362a0b28ad1ea # 0.3.0
)

set(FMT_INSTALL ON)
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        a33701196adfad74917046096bf5a2aa0ab0bb50 # 9.1.0
)

FetchContent_Declare(GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        b10fad38c4026a29ea6561ab15fc4818170d1c10 # branch: main
)

FetchContent_Declare(http
    GIT_REPOSITORY ../libhttp.git
    GIT_TAG        d4f761d67d01363dcd28d3bb5b099f0aa79db731 # 0.3.0
)

FetchContent_Declare(magix
    GIT_REPOSITORY ../magix.git
    GIT_TAG        f92eecf6ca2dce993eed9d4d9dbf2d0fefcda208 # 0.2.0
)

FetchContent_Declare(netcore
    GIT_REPOSITORY ../netcore.git
    GIT_TAG        287a0ea7c20d8549fdf01a6eb55931fdcc5d58ff # 0.5.0
)

set(JSON_Install ON)
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        bc889afb4c5bf1c0d8ee29ef35eaaf4c8bef8a5d # v3.11.2
)

FetchContent_Declare(timber
    GIT_REPOSITORY ../timber.git
    GIT_TAG        9e6fd332fc3dc80a14ad8d5476a268ea867714f0 # 0.4.0
)

FetchContent_Declare(uuidcpp
    GIT_REPOSITORY ../uuidcpp.git
    GIT_TAG        55349429b9b7f9b671f8e64a01963017880f5232 # 0.3.0
)

FetchContent_Declare(zipline
    GIT_REPOSITORY ../zipline-cpp.git
    GIT_TAG        1bb6a50deee87862961929a2ca72967e14902dad # 0.4.0
)

set(packages
    ext
    fmt
    http
    netcore
    nlohmann_json
    uuidcpp
    zipline
)

if(FSTORE_BUILD_SERVER OR PROJECT_TESTING)
    list(APPEND packages
        commline
        conftools
        cpp-postgres
        dbtools
        dmon
        magix
        timber
    )

    find_package(PkgConfig REQUIRED)

    pkg_check_modules(CRYPTOPP REQUIRED libcryptopp)
endif()

if(PROJECT_TESTING)
    list(APPEND packages GTest)
endif()

FetchContent_MakeAvailable(${packages})

cmake_policy(POP)
