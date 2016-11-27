CFLAGS= -std=c11 -O0 -Wall -Wmissing-declarations -Wreturn-type -Wunused-variable
SOURCES=ifj-inter.c \
		ifj-load.c \
		ifj-lexa.c \
		ifj-syna.c \
		ial.c \
		ifj-token.c \
		buffer.c \
		ifj-util.c \
		ifj-sema.c \
		ifj-exp.c \
		ifj-exec.c

HEADERS=ifj-inter.h \
		ifj-lexa.h \
		ifj-syna.h \
		ial.h \
		ifj-token.h \
		buffer.h \
		ifj-util.h \
		ifj-sema.h \
		ifj-exp.h \
		ifj-exec.h \
		ifj-tables.h 
CC=gcc
all: ifj16
ifj16: $(SOURCES) $(HEADERS) ifj-base.c ; $(CC) $(CFLAGS) $(SOURCES) ifj-base.c -o ifj16