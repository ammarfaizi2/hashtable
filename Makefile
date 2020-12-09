

CC       := cc
CXX      := c++
CFLAGS   := -Wall -Wextra -ggdb3
CXXFLAGS := -Wall -Wextra -ggdb3
ROOT_DIR := $(abspath $(lastword $(MAKEFILE_LIST)))

all: test

test: src/test.o


src/test.o: src/test.c
	$(CC) $(CFLAGS) -c src/test.c -o $(@)

src/hashtable/hashtable.o: src/hashtable/hashtable.c
	$(CC) $(CFLAGS) -c src/hashtable/hashtable.c -o $(@)

test.bin: src/test.o src/hashtable/hashtable.o
	$(CC) $(CFLAGS) src/test.o src/hashtable/hashtable.o -o $(@)

test: test.bin
	"$(ROOT_DIR)/test.bin"

clean:
	find "$(ROOT_DIR)" -name '*.o' | xargs rm -vf
