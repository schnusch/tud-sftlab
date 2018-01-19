cflags  = -O0 -g -Wall -Wextra -pedantic -Wshadow $(CFLAGS)
ldflags = $(LDFLAGS)

CXX ?= g++

all: rcopy

rcopy: rcopy.cc
	$(CXX) $(cflags) -o $@ $^ $(ldflags)

clean:
	$(RM) rcopy
