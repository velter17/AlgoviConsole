cmake_minimum_required(VERSION 3.5)

project (algovi_test_status)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS filesystem program_options REQUIRED)

add_executable( algovi_test_status status.cpp)

target_link_libraries(algovi_test_status
    Boost::program_options
    test_archive
    common-filesystem
    termcolor
    common-settings_reader
    )

install (TARGETS
    algovi_test_status
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
