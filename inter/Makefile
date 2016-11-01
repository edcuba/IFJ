CFLAGS= -std=c11 -O0 -Wall -Wmissing-declarations -Wreturn-type -Wunused-variable
SOURCES= ifj-base.c ifj-inter.c ifj-load.c ifj-lexa.c ifj-syna.c ial.c ifj-token.c utils/buffer.c ifj-util.c
HEADERS= ifj-inter.h ifj-lexa.h ifj-syna.h ial.h ifj-token.h utils/buffer.h ifj-util.h
TEST_SOURCES= ifj-test.c ifj-inter.c ifj-load.c ifj-lexa.c ifj-syna.c ial.c ifj-token.c utils/buffer.c ifj-util.c
CC=gcc
all: ifj16
ifj16: $(SOURCES) $(HEADERS) ; $(CC) $(CFLAGS) $(SOURCES) -o ifj16
test: $(TEST_SOURCES) $(HEADERS) ; $(CC) $(CFLAGS) $(TEST_SOURCES) -o test_ifj16
