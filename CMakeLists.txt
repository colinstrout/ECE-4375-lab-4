cmake_minimum_required(VERSION 3.10)

project(BufferProject C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(buffer_test ex_buff_test.c ex_buff.c)

target_link_libraries(buffer_test PRIVATE Threads::Threads)