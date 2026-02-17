cmake_minimum_required(VERSION 3.10)

project(BufferProject C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(inc)

add_executable(buffer_test 
    src/ex_buff_test.c 
    src/ex_buff.c
)
add executable(buffer_31
    src/ex_buff31.c
    src/ex_buff.c
)

target_link_libraries(buffer_test PRIVATE Threads::Threads)
target_link_libraries(buffer_31 PRIVATE Threads::Threads)