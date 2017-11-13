cmake_minimum_required(VERSION 3.5)

project (algovi_tester)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.63 COMPONENTS program_options REQUIRED)

add_executable( algovi_tester tester.cpp )

target_link_libraries(algovi_tester
    Boost::program_options
    tester
    executor
    compiler
    termcolor
    common-output_utils
    )

install (TARGETS
    algovi_tester
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
