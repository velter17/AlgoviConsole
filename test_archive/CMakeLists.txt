cmake_minimum_required(VERSION 3.5)

project (test_archive)

find_package(Boost 1.62 COMPONENTS filesystem REQUIRED)

set (CMAKE_CXX_STANDARD 11)

set ( TEST_ARCHIVE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestArchive.cpp
    )

add_library (${PROJECT_NAME}
    ${TEST_ARCHIVE_SOURCES}
    )

target_link_libraries(${PROJECT_NAME}
    Boost::filesystem
    common-filesystem
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

install (TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${BIN_DIR}/libs/
    )

add_subdirectory(test)
