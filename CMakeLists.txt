cmake_minimum_required (VERSION 3.5)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build_tools)
include(InstallRules)
include(Libraries)

project (someee)

find_package(Boost 1.62 EXACT REQUIRED COMPONENTS system filesystem)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_subdirectory(common)
add_subdirectory(compiler)
add_subdirectory(executor)
add_subdirectory(tester)
add_subdirectory(test_archive)
add_subdirectory(external_libs)
add_subdirectory(apps)
