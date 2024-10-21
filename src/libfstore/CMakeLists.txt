target_sources(fstore PRIVATE
    except.cpp
    object_store.cpp
    part.cpp
)

add_subdirectory(detail)
add_subdirectory(model)
add_subdirectory(sync)
