cmake_minimum_required(VERSION 3.5)

project(test)

set(CXX_STANDARD_REQUIRED 11)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
add_library(hellolib SHARED hello.cpp)

add_executable(run_main main.cpp)
target_link_libraries(run_main hellolib)