cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

add_executable(executor_test executor_test.cpp)
target_link_libraries(executor_test
    executor
    compiler
    pthread
    )

install (TARGETS executor_test
    RUNTIME DESTINATION ${BIN_DIR}/tests/
    )
