file(GLOB_RECURSE ALGOVI_LIB_HEADERS *.hpp)

add_library (algovi_lib INTERFACE)

target_include_directories(algovi_lib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

install ( DIRECTORY include/ DESTINATION /usr/local/include )
