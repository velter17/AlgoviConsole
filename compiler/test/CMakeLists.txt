cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)

add_executable(compiler_test compiler_test.cpp)
target_link_libraries(compiler_test
    compiler
    )

install (TARGETS compiler_test
    RUNTIME DESTINATION ${BIN_DIR}/tests/
    )
