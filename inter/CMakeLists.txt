cmake_minimum_required(VERSION 3.5.x)
project (IFJ C)

set(modules_sources
        ial.c
        ial.h
        ifj_lexa.c
        ifj_lexa.h
        ifj_inter.c
        ifj_inter.h
        ifj_load.c
        ifj_sema.c
        ifj_syna.c
        ifj_syna.h
        ifj_exp.h
        ifj_exp.c
        buffer.h
        buffer.c
        ifj_token.c
        ifj_token.h
        ifj_util.c
        ifj_util.h
        ifj_exec.c
        ifj_exec.h
        ifj_tables.h)

add_library(modules ${modules_sources})

add_executable(ifj16 ifj_base.c)

target_link_libraries(ifj16 modules)
