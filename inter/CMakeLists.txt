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
        utils/htable.h
        utils/htable.c
        utils/buffer.h
        utils/buffer.c
        ifj-token.c
        ifj-token.h
        ifj-util.c
        ifj-util.h)

add_library(modules ${modules_sources})

add_executable(ifj16 ifj-base.c)
add_executable(ifj16test ifj-test.c tests/lexa.c)

target_link_libraries(ifj16 modules)
target_link_libraries(ifj16test modules)
