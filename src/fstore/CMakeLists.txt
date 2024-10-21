target_sources(fstored PRIVATE
    cli.cpp
    main.cpp
)

add_subdirectory(api)
add_subdirectory(commands)
add_subdirectory(db)
add_subdirectory(options)
