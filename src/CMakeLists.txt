# src CMakeLists

add_executable(myexpr main.cpp)

add_subdirectory(frontend)
add_subdirectory(ast)
add_subdirectory(backend)

target_link_libraries(myexpr PRIVATE frontend PRIVATE ast PRIVATE backend)
