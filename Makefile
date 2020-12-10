

CC       := cc
CXX      := c++
CFLAGS   := -Wall -Wextra -ggdb3 -O3 -mavx512f
CXXFLAGS := -Wall -Wextra -ggdb3 -O3 -mavx512f

all: test.bin

test: src/test.o


src/test.o: src/test.c
	$(CC) $(CFLAGS) -c src/test.c -o $(@)

src/hashtable/hashtable.o: src/hashtable/hashtable.c src/hashtable/hashtable.h
	$(CC) $(CFLAGS) -c src/hashtable/hashtable.c -o $(@)

test.bin: src/test.o src/hashtable/hashtable.o
	$(CC) $(CFLAGS) src/test.o src/hashtable/hashtable.o -o $(@)

test: test.bin
	@./test.bin

clean:
	find -name '*.o' | xargs rm -vf
