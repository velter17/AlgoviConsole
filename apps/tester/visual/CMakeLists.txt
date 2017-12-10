cmake_minimum_required(VERSION 3.5)

project (algovi_tester_visual)

set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS program_options REQUIRED)

add_executable( algovi_tester_visual tester_visual.cpp )

target_link_libraries(algovi_tester_visual
    Boost::program_options
    tester
    executor
    compiler
    termcolor
    common-output_utils
    pthread
    ${VISUAL_VIEW_NAME}
    ${VISUAL_MODEL_NAME}
    )

install (TARGETS
    algovi_tester_visual
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
