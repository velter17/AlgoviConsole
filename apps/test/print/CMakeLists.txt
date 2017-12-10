set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS filesystem program_options REQUIRED)

add_executable(algovi_test print.cpp)

target_link_libraries(algovi_test
    Boost::program_options
    Boost::filesystem
    test_archive
    common-output_utils
    )

install (TARGETS
    algovi_test
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
