add_subdirectory(tester)
add_subdirectory(test)
add_subdirectory(algovi_lib)
add_subdirectory(generate)
add_subdirectory(validate)

install ( FILES
    algovi_start_script.sh
    algovi_complation_script.sh
    DESTINATION ${BIN_DIR}/bin
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
