find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set ( CMAKE_AUTOMOC ON )
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set ( VISUAL_MODEL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CommandVisualizer.cpp
    )

add_library ( ${VISUAL_MODEL_NAME}
    ${VISUAL_MODEL_SRC}
    )

target_link_libraries(${VISUAL_MODEL_NAME}
    visualize_inferface
    )

qt5_use_modules(${VISUAL_MODEL_NAME}
    Core
    Widgets
    )

install (TARGETS ${VISUAL_MODEL_NAME}
    ARCHIVE DESTINATION ${BIN_DIR}/libs/
    )

