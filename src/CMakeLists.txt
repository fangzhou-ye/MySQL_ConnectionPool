aux_source_directory(. SRC_LIST)
add_executable(app ${SRC_LIST})
target_link_libraries(app mysqlclient pthread)