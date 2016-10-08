cmake_minimum_required(VERSION 3.6.1)
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
        ifj-syna.h utils/htable.h utils/htable.c)

add_library(modules ${modules_sources})

add_executable(ifj16 ifj-base.c)
target_link_libraries(ifj16 modules)