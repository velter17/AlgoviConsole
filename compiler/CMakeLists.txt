cmake_minimum_required(VERSION 3.5)

project (compiler)

find_package(Boost 1.63 COMPONENTS filesystem REQUIRED)

set (CMAKE_CXX_STANDARD 11)

set ( COMPILER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Compiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SourceCode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Executable.cpp
    )

add_library (${PROJECT_NAME}
    ${COMPILER_SOURCES}
    )

target_link_libraries(${PROJECT_NAME}
    Boost::filesystem
    common-filesystem
    executor
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

install (TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${BIN_DIR}/libs/
    )

add_subdirectory(test)
