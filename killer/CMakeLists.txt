cmake_minimum_required(VERSION 3.20)
project(killer)

set(CMAKE_CXX_STANDARD 20)

add_executable(killer main.cpp)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(SOURCE_FILES main.cpp)
target_link_libraries(killer ${CMAKE_EXE_LINKER_FLAGS})

