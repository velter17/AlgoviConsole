cmake_minimum_required(VERSION 3.5)

project (algovi_test_switch)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS filesystem program_options REQUIRED)

add_executable(algovi_test_switch switch.cpp)

target_link_libraries(algovi_test_switch
    Boost::filesystem
    common-settings_reader
    common-filesystem
    )

install (TARGETS
    algovi_test_switch
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
