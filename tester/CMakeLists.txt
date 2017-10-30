cmake_minimum_required(VERSION 3.5)

project (tester)

find_package(Boost 1.63 COMPONENTS filesystem REQUIRED)

set ( CMAKE_AUTOMOC ON )
#set ( CMAKE_AUTOUIC ON )

set (CMAKE_CXX_STANDARD 11)

set ( TESTER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Tester.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Checker.cpp
    )

add_library (${PROJECT_NAME}
    ${TESTER_SOURCES}
    )

target_link_libraries(${PROJECT_NAME}
    Boost::filesystem
    compiler
    executor
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

install (TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${BIN_DIR}/libs/
    )

add_subdirectory(test)
