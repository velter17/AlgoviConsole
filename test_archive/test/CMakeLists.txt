cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic
    tester
    common-filesystem
    )

install (TARGETS test_basic
    RUNTIME DESTINATION ${BIN_DIR}/tests/
    )
