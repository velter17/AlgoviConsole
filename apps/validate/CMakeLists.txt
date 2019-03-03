set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS filesystem program_options REQUIRED)

add_executable(algovi_validate validate.cpp)

target_link_libraries(algovi_validate
    Boost::program_options
    Boost::filesystem
    test_archive
    compiler
    executor
    tester
    common-output_utils
    common-settings_reader
    )

install (TARGETS
    algovi_validate
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
