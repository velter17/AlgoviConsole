set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.63 COMPONENTS filesystem program_options REQUIRED)

add_executable(algovi_test_visual visual.cpp)

target_link_libraries(algovi_test_visual
    Boost::program_options
    Boost::filesystem
    test_archive
    common-output_utils
    compiler
    ${VISUAL_VIEW_NAME}
    ${VISUAL_MODEL_NAME}
    )

install (TARGETS
    algovi_test_visual
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
