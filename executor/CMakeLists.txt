cmake_minimum_required(VERSION 3.5)

project (executor)

find_package(Boost 1.62 COMPONENTS filesystem signals timer REQUIRED)
find_package(Qt5Core REQUIRED)

set ( CMAKE_AUTOMOC ON )
#set ( CMAKE_AUTOUIC ON )

set (CMAKE_CXX_STANDARD 11)

set ( EXECUTOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Executor.cpp
    )

add_library (${PROJECT_NAME} 
    ${EXECUTOR_SOURCES}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE Boost::filesystem
    PRIVATE Boost::signals
    PRIVATE Boost::timer
    PRIVATE pthread
    PRIVATE Qt5::Core
    PRIVATE compiler
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

install (TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${BIN_DIR}/libs/
    LIBRARY DESTINATION ${BIN_DIR}/libs
    )

add_subdirectory(test)
