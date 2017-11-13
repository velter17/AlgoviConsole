set ( VISUALIZER_APP_NAME "test_algovi_visual" )
set ( VISUALIZER_APP_SRC 
    "main.cpp"
    )

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_executable( ${VISUALIZER_APP_NAME} ${VISUALIZER_APP_SRC} )

target_link_libraries( ${VISUALIZER_APP_NAME}
    ${VISUAL_VIEW_NAME}
    ${VISUAL_MODEL_NAME}
    Qt5::Core
    Qt5::Widgets
    )

install (TARGETS
    ${VISUALIZER_APP_NAME}
    RUNTIME DESTINATION ${BIN_DIR}/bin)
