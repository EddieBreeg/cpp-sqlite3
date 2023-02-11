cmake_minimum_required(VERSION 3.0)
project(sqlite3)

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(sqlite3 STATIC src/sqlite3.c ${SRC})


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(shell src/shell.c)
target_link_libraries(shell sqlite3)

include_directories(include)


add_executable(main examples/ex.cpp)
target_link_libraries(main sqlite3)