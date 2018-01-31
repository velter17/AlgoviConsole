add_subdirectory(create)
add_subdirectory(init)
add_subdirectory(status)
add_subdirectory(print)
add_subdirectory(switch)
add_subdirectory(rm)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS program_options filesystem REQUIRED)

add_executable(algovi_test_utils test_utils.cpp)

target_link_libraries(algovi_test_utils
    Boost::program_options
    Boost::filesystem
    test_archive
    )

install (TARGETS
    algovi_test_utils
    RUNTIME DESTINATION ${BIN_DIR}/bin)
