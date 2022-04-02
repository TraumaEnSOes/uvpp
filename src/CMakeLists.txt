add_library( uvpp STATIC
    "${CMAKE_CURRENT_LIST_DIR}/Kernel.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Loop.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/libco/libco.c"
)
target_include_directories( uvpp PRIVATE "${CMAKE_CURRENT_LIST_DIR}/libco" )
set_source_files_properties( "${CMAKE_CURRENT_LIST_DIR}/libco/libco.c"
    PROPERTIES LANGUAGE C
)

add_executable( poc
    "${CMAKE_CURRENT_LIST_DIR}/poc.cpp"
)
add_dependencies( poc uvpp )
target_link_libraries( poc PRIVATE uvpp )
