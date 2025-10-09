set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(vrt_size vrt_size.c)
target_link_libraries(vrt_size PRIVATE VarType::VarType)

add_executable(vrt_offset vrt_offset.c)
target_link_libraries(vrt_offset PRIVATE VarType::VarType)