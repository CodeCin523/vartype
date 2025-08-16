add_executable(example example.c)
target_link_libraries(example PRIVATE ${VRT_NAME})

add_executable(memoryManager memoryManager.c)
target_link_libraries(memoryManager PRIVATE ${VRT_NAME})

add_executable(nameRegister nameRegister.c)
target_link_libraries(nameRegister PRIVATE ${VRT_NAME})