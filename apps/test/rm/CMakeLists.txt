set (CMAKE_CXX_STANDARD 11)

find_package(Boost 1.62 COMPONENTS filesystem program_options REQUIRED)

add_executable(algovi_test_remove remove.cpp)

target_link_libraries(algovi_test_remove
    Boost::program_options
    Boost::filesystem
    test_archive
    common-output_utils
    common-settings_reader
    )

install (TARGETS
    algovi_test_remove
    RUNTIME DESTINATION ${BIN_DIR}/bin
    )
