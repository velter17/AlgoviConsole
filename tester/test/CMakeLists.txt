cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic
    tester
    common-filesystem
    )

add_executable(tester_test tester_test.cpp)
target_link_libraries(tester_test
    tester
    executor
    common-filesystem
    )

install (TARGETS test_basic tester_test
    RUNTIME DESTINATION ${BIN_DIR}/tests/
    )
