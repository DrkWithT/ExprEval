# Top CMakeLists
# ExprEval

cmake_minimum_required(VERSION 3.27.7)

project("ExprEval")
project("ExprEval" VERSION 0.1.0)

option(USE_DEBUG_BUILD "Compile with debug info and low optimization." ON)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (USE_DEBUG_BUILD)
    add_compile_options(-Wall -Wextra -Werror -g)
else()
    add_compile_options(-Wall -Wextra -Werror -O2)
endif()

include_directories("./include")
add_subdirectory("src")
