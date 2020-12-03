

CC       := cc
CXX      := c++
CFLAGS   := -Wall -Wextra -ggdb3
CXXFLAGS := -Wall -Wextra -ggdb3


ROOT_DIR := $(abspath $(lastword $(MAKEFILE_LIST)))

export ROOT_DIR

all: test

test: src/test.o


src/test.o: src/test.c
	$(CC) $(CFLAGS) -c src/test.c -o $(@)
