set ( CMAKE_AUTOMOC ON )
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set ( VISUAL_VIEW_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MainWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsScene.cpp
    )

qt5_wrap_cpp( VISUAL_VIEW_QT_SRC
    ${PROJECT_SOURCE_DIR}/include/visualize/view/GraphicsScene.hpp
    )

add_library ( ${VISUAL_VIEW_NAME}
    ${VISUAL_VIEW_SRC}
    ${VISUAL_VIEW_QT_SRC}
    )

target_link_libraries(${VISUAL_VIEW_NAME}
    PUBLIC visualize_inferface
    PUBLIC Qt5::Core
    PUBLIC Qt5::Widgets
    )

install (TARGETS ${VISUAL_VIEW_NAME}
    ARCHIVE DESTINATION ${BIN_DIR}/libs/
    )

