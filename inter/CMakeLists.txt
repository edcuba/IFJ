cmake_minimum_required(VERSION 3.5.x)
project (IFJ C)

set(modules_sources
        ial.c
        ial.h
        ifj-lexa.c
        ifj-lexa.h
        ifj-inter.c
        ifj-inter.h
        ifj-load.c
        ifj-sema.c
        ifj-syna.c
        ifj-syna.h
        ifj-exp.h
        ifj-exp.c
        buffer.h
        buffer.c
        ifj-token.c
        ifj-token.h
        ifj-util.c
        ifj-util.h
        ifj-exec.c
        ifj-exec.h
        ifj-tables.h)

add_library(modules ${modules_sources})

add_executable(ifj16 ifj-base.c)

target_link_libraries(ifj16 modules)
