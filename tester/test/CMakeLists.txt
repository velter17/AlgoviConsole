cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

add_executable(tester_test tester_test.cpp)
target_link_libraries(tester_test
    tester
    executor
    test_archive
    common-filesystem
    )

install (TARGETS tester_test
    RUNTIME DESTINATION ${BIN_DIR}/tests/
    )
