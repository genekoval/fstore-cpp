target_sources(fstore PUBLIC FILE_SET HEADERS FILES
    event.hpp
    except.hpp
    fstore
    object_store.hpp
)

add_subdirectory(detail)
add_subdirectory(model)
add_subdirectory(net)
add_subdirectory(sync)
