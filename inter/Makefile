CFLAGS=-Wmissing-declarations -Wreturn-type -Wunused-variable
LIBS=$(shell pkg-config --cflags --libs glib-2.0)
SOURCES= ifj-base.c ifj-inter.c ifj-load.c
HEADERS= ifj-inter.h
CC=gcc
all: ifj
ifj: $(SOURCES) $(HEADERS) ; $(CC) $(CFLAGS) $(LIBS) $(SOURCES) -o ifj
