cmake_minimum_required(VERSION 3.5)

project (algovi_test_init)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS filesystem program_options REQUIRED)

add_executable( algovi_test_init init.cpp)

target_link_libraries(algovi_test_init
    Boost::program_options
    Boost::filesystem
    )

install (TARGETS
    algovi_test_init
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
