CFLAGS=-Wmissing-declarations -Wreturn-type -Wunused-variable
SOURCES= ifj-base.c ifj-inter.c ifj-load.c ifj-lexa.c ifj-syna.c ial.c
HEADERS= ifj-inter.h ifj-lexa.h ifj-syna.h ial.h
CC=gcc
all: ifj16
ifj16: $(SOURCES) $(HEADERS) ; $(CC) $(CFLAGS) $(SOURCES) -o ifj16
clean:
	rm ifj16
