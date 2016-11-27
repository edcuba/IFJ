CFLAGS= -std=c11 -O2 -Wall -Wmissing-declarations -Wreturn-type -Wunused-variable
SOURCES=ifj_inter.c \
		ifj_load.c \
		ifj_lexa.c \
		ifj_syna.c \
		ial.c \
		ifj_token.c \
		buffer.c \
		ifj_util.c \
		ifj_sema.c \
		ifj_exp.c \
		ifj_exec.c \
		ifj_base.c

HEADERS=ifj_inter.h \
		ifj_lexa.h \
		ifj_syna.h \
		ial.h \
		ifj_token.h \
		buffer.h \
		ifj_util.h \
		ifj_sema.h \
		ifj_exp.h \
		ifj_exec.h \
		ifj_tables.h 
CC=gcc
all: ifj16
ifj16: $(SOURCES) $(HEADERS) ; $(CC) $(CFLAGS) $(SOURCES) -o ifj16