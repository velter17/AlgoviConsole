set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS filesystem program_options REQUIRED)

add_executable(algovi_generate generate.cpp)

target_link_libraries(algovi_generate
    Boost::program_options
    Boost::filesystem
    test_archive
    compiler
    executor
    common-output_utils
    common-settings_reader
    )

install (TARGETS
    algovi_generate
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
