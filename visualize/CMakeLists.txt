cmake_minimum_required(VERSION 3.5)

project (visualize_inferface)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)

set (CMAKE_CXX_STANDARD 11)

add_library (${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_link_libraries(${PROJECT_NAME} 
    PUBLIC INTERFACE Qt5::Widgets
    PUBLIC INTERFACE Qt5::Core
    )

add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(test)
