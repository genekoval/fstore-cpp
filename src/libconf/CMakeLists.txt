target_sources(conf PRIVATE
    settings.cpp
    yaml.cpp
)

if(PROJECT_TESTING)
    target_sources(fstore.test PRIVATE
        settings.env.test.cpp
    )
endif()
