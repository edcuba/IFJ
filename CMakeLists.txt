cmake_minimum_required(VERSION 3.5.x)
project (IFJ C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -O0 -Wall -Wmissing-declarations -Wreturn-type -Wunused-variable")

add_subdirectory(inter)