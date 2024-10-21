target_sources(core PRIVATE
    object_store.cpp
)

add_subdirectory(db)
add_subdirectory(fs)
add_subdirectory(functions)
