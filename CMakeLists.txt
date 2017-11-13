cmake_minimum_required (VERSION 3.5)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build_tools)
include(InstallRules)
include(Libraries)

project (someee)

add_subdirectory(common)
add_subdirectory(compiler)
add_subdirectory(executor)
add_subdirectory(tester)
add_subdirectory(test_archive)
add_subdirectory(external_libs)
add_subdirectory(apps)
add_subdirectory(visualize)
