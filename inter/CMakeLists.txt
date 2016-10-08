cmake_minimum_required(VERSION 3.5.x)
project (IFJ)

set(modules_sources
        ial.c
        ial.h
        ifj-inter.c
        ifj-inter.h
        ifj-lexa.c
        ifj-lexa.h
        ifj-load.c
        ifj-sema.c
        ifj-syna.c
        ifj-syna.h)

add_library(modules ${modules_sources})

add_executable(ifj16 ifj-base.c)
add_executable(ifj16test ifj-test.c)

target_link_libraries(ifj16 modules)
target_link_libraries(ifj16test modules)