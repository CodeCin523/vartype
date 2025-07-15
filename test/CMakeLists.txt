add_executable(example example.c)
target_link_libraries(example PRIVATE ${VRT_NAME})
add_executable(mem mem.c)
target_link_libraries(mem PRIVATE ${VRT_NAME})