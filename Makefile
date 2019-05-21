CC=gcc
CFLAGS=-std=c99 
OPTS=-Werror
LDFLAGS=-llibcurl
TARGET=libsws.so

default: $(TARGET)

static: libsws.a

$(TARGET): libsws.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

libsws.a: libsws.o
	ar rcs $@ $^

libsws.o: libsws.c
	$(CC) $(CFLAGS) -c $^ -o $@
