cmake_minimum_required(VERSION 3.5)

project (common-settings_reader)

find_package(Boost 1.62 COMPONENTS filesystem REQUIRED)

set (CMAKE_CXX_STANDARD 11)

add_library (${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
target_link_libraries(${PROJECT_NAME}
    INTERFACE Boost::filesystem
    )


