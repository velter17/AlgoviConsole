cmake_minimum_required(VERSION 3.5)

project (algovi_tester)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS program_options REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

add_executable( algovi_tester tester.cpp )

target_link_libraries(algovi_tester
    Boost::program_options
    tester
    executor
    compiler
    termcolor
    common-output_utils
    Qt5::Core
    Qt5::Widgets
    )

install (TARGETS
    algovi_tester
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
